#pragma once

#include "ofMain.h"

class video_player : public ofBaseApp
{
  public:
    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void windowResized   (ofResizeEventArgs&);
    void messageReceived (ofMessage&);
};
