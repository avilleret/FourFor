#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"

struct SafePlayer : public ofxOMXPlayer,
    public std::mutex
{
  void draw_safe()
  {
    if(m_enable)
    {
      lock();
      int w = getWidth();
      int h = getHeight();

      int draw_w;
      int draw_h;

      if(float(w)/float(h) > 1.)
      {
        draw_w = ofGetWidth();
        draw_h = float(h)/float(w) * draw_w;
      }
      else
      {
        draw_h = ofGetHeight();
        draw_w = float(w*ofGetHeight())/ofGetWidth();
      }
      ofxOMXPlayer::draw(0, 0, draw_w, draw_h);
      unlock();
    }
  }
  std::atomic<bool> m_enable{false};
};
