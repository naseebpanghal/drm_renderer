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
};
#endif
