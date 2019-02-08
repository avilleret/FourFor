#pragma once

#include "ofMain.h"
#include "oscilloscope.h"
#include <ossia-cpp/ossia-cpp98.hpp>
#include "OssiaText.h"

class biomedical_display
{
  public:
    biomedical_display(const opp::node& root);
    ~biomedical_display();
    void update ();
    void draw   (int x, int y, int w, int h);

    void messageReceived (ofMessage&);
    void windowResized   (ofResizeEventArgs&);

    std::map<std::string, std::pair<std::string, float>> m_uniform_map;

  private:
    std::vector<std::shared_ptr<Oscilloscope>> m_oscillos;
    opp::node m_root;

    ofFbo m_fbo;
    OssiaText m_label;
    std::atomic<bool> m_enable{true};

    ofVec2f render_size;
};
