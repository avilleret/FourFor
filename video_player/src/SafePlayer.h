#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"

struct SafePlayer : public ofxOMXPlayer,
    public std::mutex
{
  SafePlayer(bool sd)
    : m_sd(sd)
  {
    ofxOMXPlayerSettings settings;
    settings.useHDMIForAudio = !m_sd;
    settings.initialVolume = 1.0;

    setup(settings);
  }

  void draw_safe(int targetWidth, int targetHeight)
  {
    if(m_enable)
    {
      ofPushStyle();

      lock();
      float w = getWidth();
      float h = getHeight();

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

      // ofLogNotice("safePlayer") << "position: " << m_position << " w x h " << draw_w << " x " << draw_h;

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
      m_enable = true;
      m_file_path_changed = false;
      unlock();
    }
  }

  std::atomic<bool> m_enable{true};
  ofFloatColor m_color{ofColor::white};


  std::atomic<bool> m_file_path_changed;
  std::string m_file_path;

  ofVec2f m_position;
  float m_scale;
  bool m_sd;
};
