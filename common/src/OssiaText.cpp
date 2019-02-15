#include "OssiaText.h"

OssiaText::OssiaText(const opp::node& root)
  : AbstractText(root)
{
  load("verdana.ttf",96);
}

void OssiaText::draw()
{
  begin();
    drawString(m_text, 0., 0.);
  end();
}

AbstractText::AbstractText(const opp::node& root)
  : std::mutex()
  , m_root(root)
{
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
    n.set_value(1.);
    n.set_value_callback([](void* ctx, const opp::value& v)
    {
      OssiaText* ptr = static_cast<OssiaText*>(ctx);
      auto s = v.to_float();
      ptr->set_scale(s);
    }, this);
  }
  {
    auto n = m_root.create_argb("color");
    n.set_value(opp::value::vec4f{1.,1.,1.,1.});
    n.set_value_callback([](void* ctx, const opp::value& v)
    {
      OssiaText* ptr = static_cast<OssiaText*>(ctx);
      auto c = v.to_vec4f();
      ptr->set_color(ofFloatColor(c[1],c[2],c[3],c[0]));
    }, this);
  }
}

void AbstractText::set_text(const std::string& s)
{
  lock();
  m_text = s;
  // auto rect = getStringBoundingBox(m_text,0,0);
  // m_center = ofVec2f(rect.width/2., rect.height/2.);
  unlock();
}

void AbstractText::begin()
{
  ofPushStyle();
  ofPushMatrix();
  lock();
  ofSetColor(m_color);
  ofTranslate(m_position.x, m_position.y);
  ofScale(m_scale,m_scale);
}

void AbstractText::end(){
  unlock();
  ofPopMatrix();
  ofPopStyle();
}

void AbstractText::set_position(const ofVec2f& pos)
{
  try
  {
    lock();
    m_position = pos;
    unlock();
  }
  catch (const std::exception& e)
  {
    ofLogError("AbstractText") << "Error in set_position() : " << e.what();
  }
}
void AbstractText::set_scale(float f)
{
  try{
  lock();
  m_scale = f;
  unlock();
  }
  catch (const std::exception& e)
  {
    ofLogError("AbstractText") << "Error in set_scale() : " << e.what();
  }
}
void AbstractText::set_color(const ofFloatColor& c)
{
  try{
    lock();
    m_color = c;
    unlock();
  }
  catch (const std::exception& e)
  {
    ofLogError("AbstractText") << "Error in set_scale() : " << e.what();
  }
}

OssiaMultiText::OssiaMultiText()
{
  load("verdana.ttf",96);
}

AbstractText& OssiaMultiText::add(const opp::node& root)
{
  m_texts.push_back(std::make_unique<AbstractText>(root));
  return *m_texts.back();
}

void OssiaMultiText::draw()
{
  for(auto& txt : m_texts)
  {
    txt->begin();
      drawString(txt->get_text(), 0., 0.);
    txt->end();
  }
}
