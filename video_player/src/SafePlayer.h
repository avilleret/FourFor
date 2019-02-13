#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"

struct SafePlayer : public ofxOMXPlayer,
    public std::mutex
{
  void draw_safe(int targetWidth, int targetHeight)
  {
    if(m_enable)
    {
      ofPushStyle();

      lock();
      float w = getWidth();
      float h = getHeight();

      float draw_w;
      float draw_h;

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

      ofSetColor(m_color);
      ofxOMXPlayer::draw(m_position.x, m_position.y, draw_w, draw_h);
      unlock();

      ofPopStyle();
    }
  }

  void update()
  {
    if(m_file_path_changed)
    {
      lock();
      ofLogNotice() << "load movie: " << m_file_path;
      loadMovie(m_file_path);
      m_enable=true;
      m_file_path_changed = false;
      unlock();
    }
  }

  std::atomic<bool> m_enable{true};
  ofFloatColor m_color{ofColor::white};


  std::atomic<bool> m_file_path_changed;
  std::string m_file_path;

  ofVec2f m_position;
};
