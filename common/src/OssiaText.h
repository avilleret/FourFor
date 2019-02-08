#pragma once

#include "ofMain.h"
#include <ossia-cpp/ossia-cpp98.hpp>

class OssiaText : public ofTrueTypeFont, public std::mutex
{
  public:
    OssiaText(const opp::node& root);

    void set_text(const std::string& s);
    void set_position(const ofVec2f& pos) { lock(); m_position = pos; unlock(); }
    void set_scale(float f){ lock(); m_scale = f; unlock(); }
    void set_color(const ofFloatColor& c){ lock(); m_color = c; unlock(); }
    void draw();

  private:
    opp::node m_root;
    std::string m_text;
    ofVec2f m_position;
    ofVec2f m_center;
    float m_scale{1.};
    ofFloatColor m_color{ofFloatColor::white};
};
