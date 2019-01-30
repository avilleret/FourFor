#pragma once

#include "ofMain.h"
#include "oscilloscope.h"
#include "ofxShadertoy.h"
#include <ossia-cpp/ossia-cpp98.hpp>  

class biomedical_display : public ofBaseApp
{
  public:
    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void messageReceived (ofMessage&);
    void windowResized   (ofResizeEventArgs&);

    ofxShadertoy& get_shader() { return m_shader; }

    std::map<std::string, std::pair<std::string, float>> m_uniform_map;

  private:
    std::vector<Oscilloscope> m_oscillos;
    opp::oscquery_server m_server;

    ofxShadertoy m_shader;
    ofFbo m_fbo;
   ofFbo m_PALfbo;

   ofVec2f render_size;
};
