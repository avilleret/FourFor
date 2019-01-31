#include "biomedical_display.h"

int main()
{
#ifdef TARGET_RASPBERRY_PI
  ofGLESWindowSettings settings;
  settings.glesVersion = 2;
  settings.setSize(1920,1080);
#else
  ofGLWindowSettings settings;
  settings.setGLVersion(3,2);
  settings.setSize(1024,768);
#endif

  auto window = ofCreateWindow(settings);
  auto app = make_shared<biomedical_display>();
  ofRunApp(window, app);

  return ofRunMainLoop();
}
