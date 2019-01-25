#include "oscilloscope.h"

Oscilloscope::Oscilloscope(const opp::node& root)
  : m_root(root)
{
  m_buffer.resize(1024);
  for(size_t i = 0; i < m_buffer.size(); i++)
  {
    m_buffer[i] = ofDefaultVertexType(i, 0.5, 0.);
  }

  {
    auto n = m_root.create_argb("color");
    opp::value::vec4f c{1.,1.,0.,0.};
    n.set_value(c);
    n.set_value_callback(
          [](void* context, const opp::value& v){
      Oscilloscope* osc = (Oscilloscope*) context;
      auto vec = v.to_vec4f();
      osc->set_color(ofColor(vec[1],vec[2],vec[3],vec[0]));
    }, this);
  }

  {
    auto n = m_root.create_bool("enable");
    n.set_value(true);
    n.set_value_callback(
          [](void* context, const opp::value& v){
      Oscilloscope* osc = (Oscilloscope*) context;
      auto b = v.to_bool();
      osc->set_enabled(b);
    }, this);
  }

  {
    auto n = m_root.create_float("input");
    n.set_value(0.);
    n.set_value_callback(
          [](void* context, const opp::value& v){
      Oscilloscope* osc = (Oscilloscope*) context;
      auto f = v.to_float();
      osc->set_value(f);
    }, this);
  }

}

void Oscilloscope::update()
{
  m_buffer[m_index] = ofDefaultVertexType(m_index, m_value/2.+0.5, 0);
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
