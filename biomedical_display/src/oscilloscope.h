#pragma once

#include <ofMain.h>

class Oscilloscope {

  public:
    Oscilloscope();

    void draw(float x, float y, float w, float h);
    void update(float f);
    void setColor(ofColor c){ m_color=c; }

  private:
    std::vector<ofDefaultVertexType> m_buffer;
    size_t m_index{0};
    ofColor m_color{ofColor::red};
    float m_line_width{1.};
};
