#pragma once

#include "ofMain.h"
#include "oscilloscope.h"

class ofApp : public ofBaseApp
{
  public:
    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void messageReceived (ofMessage&);

  private:
    std::vector<Oscilloscope> m_oscillos;
};
