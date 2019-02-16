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
        ofPushStyle();
        lock();
        ofSetColor(m_color);
        draw(x,y,w,h);
        unlock();
        ofPopStyle();
      }
    }
    void draw_safe(float x, float y)
    {
      int targetWidth;
      int targetHeight;

      float w = ofImage::getWidth();
      float h = ofImage::getHeight();

      float draw_w = ofGetWidth();
      float draw_h = ofGetHeight();

      if(!m_sd)
      {
        float ratio = w/h;
        float target_ratio = float(targetWidth)/float(targetHeight);

        if(ratio > target_ratio)
        {
          draw_w = targetWidth;
          draw_h = draw_w / ratio;
        }
        else
        {
          draw_h = targetHeight;
          draw_w = draw_h * ratio;
        }
      }

      draw_safe(x, y, m_scale*draw_w, m_scale*draw_h);
    }
    void draw_safe()
    {
      draw_safe(m_position[0],m_position[1]);
    }

    void update()
    {
      if(m_changed)
      {
        if(load(std::string("images/" + m_file)))
        {
          ofLogNotice() << "load " << m_file;
          m_enable=true;
        }
        else
        {
          ofLogError() << "can't load image " << m_file;
        }
        m_changed=false;
      }
    }

    opp::value::vec2f m_position{0.,0.};
    std::atomic<float> m_scale{1.};
    std::atomic<bool> m_enable{true};
    std::atomic<bool> m_changed{false};
    ofFloatColor m_color{ofColor::white};

    std::string m_file;
    bool m_sd;
};
