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
      lock();
      float w = getWidth();
      float h = getHeight();

      float draw_w;
      float draw_h;

      if(w/h > 1.f)
      {
        draw_w = targetWidth;
        draw_h = h * draw_w/w;
      }
      else
      {
        draw_h = targetHeight;
        draw_w = w * draw_h/h;
      }
      ofxOMXPlayer::draw(0, 0, draw_w, draw_h);
      unlock();
    }
  }
  std::atomic<bool> m_enable{true};
};
