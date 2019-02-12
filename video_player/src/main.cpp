#include "video_player.h"

int main()
{
#ifdef TARGET_RASPBERRY_PI
  ofGLESWindowSettings settings;
  settings.glesVersion = 2;
  // settings.setSize(ofGetScreenWidth(),ofGetScreenHeight());
  settings.setSize(1920,1080);
#else
  ofGLWindowSettings settings;
  settings.setGLVersion(3,2);
  settings.setSize(1024,768);
#endif

  string hostname;
  string filepath("/etc/hostname");
  ifstream file (filepath);
  if (file.is_open())
  {
    if ( getline (file,hostname) )
    {
      ofLogNotice() << "hostname : " << hostname;
    }
    else {
      ofLogError() << "can't read " << filepath;
    }
    file.close();
  }

  auto window = ofCreateWindow(settings);
  auto app = make_shared<video_player>(hostname);
  ofRunApp(window, app);

  return ofRunMainLoop();
}
