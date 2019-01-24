#include "oscilloscope.h"

Oscilloscope::Oscilloscope()
{
  m_buffer.resize(1024);
  for(size_t i = 0; i < m_buffer.size(); i++)
  {
    m_buffer[i] = ofDefaultVertexType(i, 0.5, 0.);
  }
}

void Oscilloscope::update(float f)
{
  m_buffer[m_index] = ofDefaultVertexType(m_index, f/2.+0.5, 0);
  m_index = (m_index + 1) % m_buffer.size();
}

void Oscilloscope::draw(float x, float y, float w, float h)
{
  ofPushMatrix();
  ofTranslate(x*ofGetWidth(),y*ofGetHeight());
  ofScale(w,h);
  ofScale(float(ofGetWidth())/float(m_buffer.size()), ofGetHeight());

  ofPushStyle();
  ofSetColor(m_color);
  ofSetLineWidth(m_line_width);

  ofPolyline line(m_buffer);
  line.setClosed(false);
  line.draw();

  ofPopStyle();
  ofPopMatrix();
}
