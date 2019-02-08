#include "oscilloscope.h"
#include "ofxFatLine.h"

Oscilloscope::Oscilloscope(const opp::node& root)
  : m_root(root)
{
  unsigned long size = 1024;
  m_buffer.resize(size);
  m_colors.resize(size);
  m_weights.resize(size);
  for(size_t i = 0; i < m_buffer.size(); i++)
  {
    m_buffer[i] = ofDefaultVertexType(float(i)/m_buffer.size(), 0.5, 0.);
    m_colors[i] = m_color;
    m_weights[i] = m_line_width;
  }

  {
    auto n = m_root.create_argb("color");
    opp::value::vec4f c{1.,1.,0.,0.};
    n.set_value(c);
    n.set_value_callback(
          [](void* context, const opp::value& v){
      Oscilloscope* osc = static_cast<Oscilloscope*>(context);
      auto vec = v.to_vec4f();
      osc->set_color(ofFloatColor(vec[1],vec[2],vec[3],vec[0]));
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

  {
    auto n = m_root.create_float("line_width");
    n.set_value(1.);
    n.set_value_callback(
          [](void* context, const opp::value& v){
      Oscilloscope* osc = (Oscilloscope*) context;
      float f = v.to_float();
      osc->m_line_width = f;
      osc->m_line_width_changed=true;
    }, this);
  }

}

void Oscilloscope::update()
{
  if(m_line_width_changed)
  {
    for(auto& w : m_weights)
    {
      w = static_cast<double>(m_line_width);
    }
    m_line_width_changed = false;
  }
  m_buffer[m_index] = ofDefaultVertexType(static_cast<float>(m_index)/m_buffer.size(), static_cast<double>(m_value)/2.+0.5, 0);
  m_index = (m_index + 1) % m_buffer.size();
}

void Oscilloscope::draw(float x, float y, float w, float h)
{
  ofPushStyle();
  ofPushMatrix();
  ofTranslate(x,y);
  ofVec2f scale(w-h,h);
  //ofScale(float(w-h)/float(m_buffer.size()),h);

  // line.setFeather(2.);
  std::vector<ofDefaultVec3> scaled_buffer;
  scaled_buffer.reserve(m_buffer.size());
  //ofLogNotice() << "start";
  for(auto d : m_buffer)
  {
    ofDefaultVec3 pt;
    pt.x = d.x * scale.x;
    pt.y = d.y * scale.y;
    //ofLogNotice() << pt;
    scaled_buffer.push_back(pt);
  }
  //ofLogNotice() << "end";
  m_mutex.lock();
  ofxFatLine line(scaled_buffer, m_colors, m_weights);
  ofSetColor(m_color);
  m_mutex.unlock();
  line.setClosed(false);
  line.draw();

  ofPopMatrix();

  ofPushMatrix();
  ofTranslate(w-h,y);
  ofDrawBitmapString("60", 20, 20, 0);

  ofPopMatrix();
  ofPopStyle();
}
