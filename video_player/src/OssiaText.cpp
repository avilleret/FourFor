#include "OssiaText.h"

OssiaText::OssiaText(const opp::node& root) :
  m_root(root)
{
  load("verdana.ttf",96);
  {
    auto n = m_root.create_string("text");
    n.set_value_callback([](void* ctx, const opp::value& v)
    {
      OssiaText* ptr = static_cast<OssiaText*>(ctx);
      auto s = v.to_string();
      ptr->set_text(s);
    }, this);
  }
  {
    auto n = m_root.create_cart2D("position");
    n.set_value_callback([](void* ctx, const opp::value& v)
    {
      OssiaText* ptr = static_cast<OssiaText*>(ctx);
      auto vec = v.to_vec2f();
      ptr->set_position(ofVec2f(vec[0], vec[1]));
    }, this);
  }
  {
    auto n = m_root.create_float("scale");
    n.set_value_callback([](void* ctx, const opp::value& v)
    {
      OssiaText* ptr = static_cast<OssiaText*>(ctx);
      auto s = v.to_float();
      ptr->set_scale(s);
    }, this);
    n.set_value(1.);
  }
}

void OssiaText::set_text(const std::string& s)
{
  lock();
  m_text = s;
  // auto rect = getStringBoundingBox(m_text,0,0);
  // m_center = ofVec2f(rect.width/2., rect.height/2.);
  unlock();
}

void OssiaText::draw()
{
  ofPushMatrix();
  lock();
  ofTranslate(m_position.x, m_position.y);
  ofScale(m_scale,m_scale);
  drawString(m_text, m_center.x, m_center.y);
  unlock();
  ofPopMatrix();
}
