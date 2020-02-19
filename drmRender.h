#ifndef __DRM_RENDER_H__
#define __DRM_RENDER_H__

#include <string>

class drmRender
{
  public:
  drmRender();
  ~drmRender();
  int init();
  private:
    std::string devName {"/dev/dri/card0"};
    int m_fd;
    uint32_t m_crtc_id, plane_id {0};
    int m_crtc_width;
    int m_crtc_height;
};
#endif
