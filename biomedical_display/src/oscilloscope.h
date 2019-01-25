#pragma once

#include <ofMain.h>
#include <ossia-cpp/ossia-cpp98.hpp>

class Oscilloscope {

  public:
    Oscilloscope(const opp::node& root);

    void draw(float x, float y, float w, float h);
    void update();
    void set_color(ofColor c){ m_color=c; }
    void set_enabled(bool b){ m_enabled=b; }
    void set_value(float f){ m_value=f; }

  private:
    std::vector<ofDefaultVertexType> m_buffer;
    size_t m_index{0};
    ofColor m_color{ofColor::red};
    bool m_enabled;
    float m_value;
    float m_line_width{1.};

    opp::node m_root;
};
