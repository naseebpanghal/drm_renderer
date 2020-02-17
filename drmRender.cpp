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
  m_fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
  assert(m_fd >= 0); 

  drmModeRes *resources = drmModeGetResources(m_fd);

  drmModeConnector *connector;
  for (int i = 0; i < resources->count_connectors; ++i) {
    connector = drmModeGetConnector(m_fd, resources->connectors[i]);
    if (!connector)
      continue;
    if (connector->connection == DRM_MODE_CONNECTED && connector->count_modes > 0) {
      printf("CONNECTOR type: %d id: %d\n", connector->connector_type, connector->connector_id);
      break;
    }
    drmModeFreeConnector(connector);
  }

  return 0;
}
