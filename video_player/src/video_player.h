#pragma once

#include "ofMain.h"
#include <ossia-cpp/ossia-cpp98.hpp>
#ifdef TARGET_RASPBERRY_PI
#include "SafePlayer.h"
#endif
#include "SafeImage.h"
#include "OssiaText.h"
#include "biomedical_display.h"

#define NUM_IMG 4

class video_player : public ofBaseApp
{
  public:
    video_player(const std::string& name);

    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void windowResized   (ofResizeEventArgs&);
    void messageReceived (ofMessage&);

    safe_image m_images[NUM_IMG];
#if TARGET_RASPBERRY_PI
    SafePlayer m_player;
#endif
    opp::oscquery_server m_server;
    ofShader m_shader;
    ofFbo m_draw_fbo, m_prev, m_curr;
    biomedical_display m_ecg;

    OssiaText m_clock;
};
