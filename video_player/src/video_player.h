#pragma once

#include "ofMain.h"
#include <ossia-cpp/ossia-cpp98.hpp>
#include "SafePlayer.h"
#include "SafeImage.h"
#include "OssiaText.h"

#define NUM_IMG 4

class video_player : public ofBaseApp
{
  public:
    video_player();

    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void windowResized   (ofResizeEventArgs&);
    void messageReceived (ofMessage&);

    safe_image m_images[NUM_IMG];
    SafePlayer m_player;
    opp::oscquery_server m_server;

    OssiaText m_clock;
};
