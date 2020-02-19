/*
 * Description: 
 * Created By: Naseeb Panghal
 * Creation Date: Feb 17, 2020
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>
#include <string.h>
#include <iostream>

#include "drmRender.h"

drmRender::drmRender()
{
}

drmRender::~drmRender()
{
}

//Temporary function, need to removed later and move to constructor
int drmRender::init()
{
  int i,j;
  int ret;
  m_fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
  assert(m_fd >= 0); 

  drmModeRes *resources = drmModeGetResources(m_fd);

  drmModeConnector *connector;
  for (i = 0; i < resources->count_connectors; ++i) 
  {
    connector = drmModeGetConnector(m_fd, resources->connectors[i]);
    if (!connector)
      continue;
    if (connector->connection == DRM_MODE_CONNECTED && connector->count_modes > 0) 
    {
      printf("CONNECTOR type: %d id: %d\n", connector->connector_type, connector->connector_id);
      break;
    }
    drmModeFreeConnector(connector);
  }

  drmModeEncoder *encoder;
  for (i = 0; i < resources->count_encoders; ++i) 
  {
    encoder = drmModeGetEncoder(m_fd, resources->encoders[i]);
    if (!encoder)
      continue;
    if (encoder->encoder_id == connector->encoder_id) 
    {
      printf("ENCODER id: %d\n", encoder->encoder_id);
      break;
    }
    drmModeFreeEncoder(encoder);
  }
  assert(i < resources->count_encoders);

  drmModeCrtcPtr crtc;
  for (i = 0; i < resources->count_crtcs; ++i) 
  {
    if (resources->crtcs[i] == encoder->crtc_id) 
    {
      crtc = drmModeGetCrtc(m_fd, resources->crtcs[i]);
      assert(crtc);
      break;
    }
  }
  assert(i < resources->count_crtcs);
  m_crtc_id = crtc->crtc_id;
  m_crtc_width = crtc->width;
  m_crtc_height = crtc->height;
  uint32_t crtc_bit = (1 << i);

  ret = drmSetClientCap(m_fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1); // not needed (overlays, primary, cursor)
  assert(!ret);
  drmModePlaneRes *plane_resources =  drmModeGetPlaneResources(m_fd);
  assert(plane_resources);

  drmModePlane *ovr;
  for (i = 0; i < plane_resources->count_planes; i++) 
  {
    ovr = drmModeGetPlane(m_fd, plane_resources->planes[i]);
    if (!ovr)
      continue;

    for (j=0; j<ovr->count_formats; j++)
    {
      if (ovr->formats[j] ==  DRM_FORMAT_NV12) 
      {
        break;
      }
    }
    if (j==ovr->count_formats)
    {
      continue;
    }

    if ((ovr->possible_crtcs & crtc_bit) && !ovr->crtc_id) 
    {
      drmModeObjectPropertiesPtr props = drmModeObjectGetProperties(m_fd, plane_resources->planes[i], DRM_MODE_OBJECT_PLANE);
      if (!props) 
      {
        continue;
      }

      for (j = 0; j < props->count_props && !plane_id; j++) 
      {
        drmModePropertyPtr prop = drmModeGetProperty(m_fd, props->props[j]);
        if (!prop) 
        {
          continue;
        }
        std::cout << "prop->name: " << prop->name << std::endl;
        if (!strcmp(prop->name, "type") && props->prop_values[j] == DRM_PLANE_TYPE_OVERLAY) 
        {
          plane_id = ovr->plane_id;
        }
        drmModeFreeProperty(prop);
      }
      drmModeFreeObjectProperties(props);
      if (plane_id) 
      {
        break;
      }
    }
    drmModeFreePlane(ovr);
  }
  assert(plane_id);

  return 0;
}
