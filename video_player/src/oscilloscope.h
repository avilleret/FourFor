#pragma once

#include <ofMain.h>
#include <ossia-cpp/ossia-cpp98.hpp>

class Oscilloscope {

  public:
    Oscilloscope(const opp::node& root);

    void draw(float x, float y, float w, float h);
    void update();
    void set_color(ofFloatColor c){
      m_mutex.lock();
      m_color=c;
      m_color_changed=true;
      m_mutex.unlock();
    }
    void set_enabled(bool b){ m_enabled=b; }
    void set_value(float f){
      m_mutex.lock();
      m_value=f;
      m_mutex.unlock();
    }

  private:
    std::vector<ofDefaultVertexType> m_buffer{};
    std::vector<double> m_weights{};
    size_t m_index{0};
    std::vector<ofFloatColor> m_colors{};
    ofFloatColor m_color{ofFloatColor::red};
    bool m_enabled;
    std::atomic<float> m_value{};
    std::atomic<float> m_line_width{1.};
    std::atomic<bool> m_line_width_changed;
    std::atomic<bool> m_color_changed;
    std::atomic<bool> m_size_changed{true};
    std::mutex m_mutex{};
    std::size_t m_size{64};

    opp::node m_root;
    int m_vbar_width=20;
};
