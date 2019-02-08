#pragma once

#include "ofMain.h"
#include "oscilloscope.h"
#include "ofxShadertoy.h"
#include <ossia-cpp/ossia-cpp98.hpp>
#include "OssiaText.h"

class biomedical_display : public ofBaseApp
{
  public:
    biomedical_display();
    void setup  ();
    void update ();
    void draw   ();
    void exit   ();

    void messageReceived (ofMessage&);
    void windowResized   (ofResizeEventArgs&);
    ofShader& get_shader() { return m_shader; }

    std::map<std::string, std::pair<std::string, float>> m_uniform_map;

  private:
    std::vector<std::shared_ptr<Oscilloscope>> m_oscillos;
    opp::oscquery_server m_server;

    ofShader m_shader;

   ofFbo m_fbo;
   ofFbo m_PALfbo;

   OssiaText m_label;

   ofVec2f render_size;
};
