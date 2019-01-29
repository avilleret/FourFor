#include "biomedical_display.h"

int main()
{
  ofGLWindowSettings settings;
  settings.setGLVersion(3,2);
  settings.setSize(1024,768);

  auto window = ofCreateWindow(settings);
  auto app = make_shared<biomedical_display>();
  ofRunApp(window, app);

  return ofRunMainLoop();
}
