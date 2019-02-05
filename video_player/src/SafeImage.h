#pragma once
#include "ofMain.h"
#include <ossia-cpp/ossia-cpp98.hpp>

class safe_image : public ofImage,
                   public std::mutex
{
  public:
    void draw_safe(float x, float y, float w, float h){
      if(m_enable)
      {
        lock();
        draw(x,y,w,h);
        unlock();
      }
    }
    void draw_safe(float x, float y)
    {
      draw_safe(x, y, m_scale*getWidth(), m_scale*getHeight());
    }
    void draw_safe()
    {
      draw_safe(m_position[0],m_position[1]);
    }

    opp::value::vec2f m_position;
    std::atomic<float> m_scale{1.};
    std::atomic<bool> m_enable{false};
    std::atomic<bool> changed{false};
};
