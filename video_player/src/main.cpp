#include "video_player.h"

int main()
{
  ofGLWindowSettings settings;

  auto window = ofCreateWindow(settings);
  auto app = make_shared<video_player>();
  ofRunApp(window, app);

  return ofRunMainLoop();
}
