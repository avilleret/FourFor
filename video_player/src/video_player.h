#pragma once

#include "ofMain.h"
#include <ossia-cpp/ossia-cpp98.hpp>

class video_player : public ofBaseApp
{
  public:
    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void windowResized   (ofResizeEventArgs&);
    void messageReceived (ofMessage&);

    opp::oscquery_server m_server;
};
