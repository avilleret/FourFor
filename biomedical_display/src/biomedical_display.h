#pragma once

#include "ofMain.h"
#include "oscilloscope.h"
#include "ofxShadertoy.h"
#include <ossia-cpp/ossia-cpp98.hpp>
#include "OssiaText.h"
#include "OssiaShader.h"
#include "SafeImage.h"

#define NUM_IMG 1
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

    std::map<std::string, std::pair<std::string, float>> m_uniform_map;

  private:
    std::vector<std::shared_ptr<Oscilloscope>> m_oscillos;
    opp::oscquery_server m_server;

    OssiaShader m_shader;

    ofFbo m_draw_fbo;
    ofFbo m_curr;
    ofFbo m_prev;

    OssiaMultiText m_texts;

    safe_image m_images[NUM_IMG];

    ofVec2f render_size;
};
