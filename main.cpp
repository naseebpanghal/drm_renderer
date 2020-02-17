#include <memory>
#include <thread>
#include <chrono>
#include "drmRender.h"
int main()
{
  std::unique_ptr<drmRender> render = std::make_unique<drmRender>();
  render->init();

  while(1)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}
