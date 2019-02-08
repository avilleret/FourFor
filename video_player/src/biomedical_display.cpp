#include "biomedical_display.h"

#define UNIFORM_NODE(node_name, uniform_name) \
{ \
  auto root = m_server.get_root_node(); \
  auto n = root.create_float(node_name); \
  n.set_value_callback([](void* ctx, const opp::value& v){ \
    biomedical_display* ptr = static_cast<biomedical_display*>(ctx); \
    float f = v.to_float(); \
    ptr->m_uniform_map[node_name].second = f; \
  }, this); \
  m_uniform_map[node_name] = std::pair<std::string, float>(uniform_name, 0.); \
  n.set_value(0); \
}

biomedical_display::biomedical_display(const opp::node& root)
  : m_root(root)
  , m_label(m_root.create_void("label"))
{
  ofSetBackgroundColor(ofColor::black);
  ofHideCursor();

  // ofAddListener(ofEvents().update, this, &biomedical_display::update);

  // ofSetLogLevel(ofLogLevel::OF_LOG_VERBOSE);

  auto n = m_root.create_bool("enable");
  n.set_value_callback([](void* ctx, const opp::value& v)
  {
    biomedical_display* ptr = (biomedical_display*)ctx;
    ptr->m_enable=v.to_bool();
  }, this);
  n.set_value(true);

  for(int i=0; i<3; i++)
  {
    m_oscillos.push_back(std::make_shared<Oscilloscope>(m_root.create_void("oscillo.1")));
  }

  m_oscillos[0]->set_color(ofColor::green);
  m_oscillos[1]->set_color(ofColor::yellow);
  m_oscillos[2]->set_color(ofColor::aqua);

  ofResizeEventArgs size(ofGetWidth(), ofGetHeight());
  windowResized(size);

  m_label.loadFont("verdana.ttf", 24);
  m_label.set_color(ofFloatColor::aqua);
  m_label.set_scale(0.5);
  m_label.set_position(ofVec2f(20., 20.));
  m_label.set_text("MORTON FELDMAN");
}

biomedical_display::~biomedical_display()
{
  //ofRemoveListener(ofEvents().update, this, &biomedical_display::update);
}

void biomedical_display::update()
{
  if(m_enable)
  {
    for(auto& osc : m_oscillos)
    {
      // osc->set_value(ofRandomf());
      osc->update();
    }

    m_fbo.begin();
    ofClear(0,0,0,0);
    int i=0;
    for(auto& osc : m_oscillos)
    {
      osc->draw(0.,float(i)/m_oscillos.size()*render_size.y,
                render_size.x,float(render_size.y)/m_oscillos.size());
      i++;
      m_label.draw();
    }
    m_fbo.end();
  }
}

void biomedical_display::draw(int x, int y, int w, int h)
{
  if(m_enable)
   m_fbo.draw(x,y,w,h);
}

void biomedical_display::windowResized   (ofResizeEventArgs& args)
{
  render_size.y = 576;
  render_size.x = float(ofGetWidth()*render_size.y)/ofGetHeight();

  m_fbo.allocate(render_size.x, render_size.y, GL_RGBA);
  m_fbo.begin();
  ofClear(ofColor::black);
  m_fbo.end();
}

