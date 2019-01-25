#include "biomedical_display.h"

int main()
{
  ofGLWindowSettings settings;

  auto window = ofCreateWindow(settings);
  auto app = make_shared<biomedical_display>();
  ofRunApp(window, app);

  return ofRunMainLoop();
}
