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

biomedical_display::biomedical_display()
  : m_server("biomedical_display", 1234, 5678)
  , m_label(m_server.get_root_node().create_void("label"))
{

}

void biomedical_display::setup()
{
  ofSetBackgroundColor(ofColor::black);
  ofHideCursor();
  // ofSetLogLevel(ofLogLevel::OF_LOG_VERBOSE);

  auto root = m_server.get_root_node();

  for(int i=0; i<3; i++)
  {
    m_oscillos.push_back(std::make_shared<Oscilloscope>(root.create_void("oscillo.1")));
  }

  m_oscillos[0]->set_color(ofColor::green);
  m_oscillos[1]->set_color(ofColor::yellow);
  m_oscillos[2]->set_color(ofColor::aqua);

  ofDisableArbTex();
  m_shader.load("shaders/distorted TV");

  ofResizeEventArgs size(ofGetWidth(), ofGetHeight());
  windowResized(size);

  UNIFORM_NODE("vertical_jerk","vertJerkOpt")
  UNIFORM_NODE("vertical_movement", "vertMovementOpt")
  UNIFORM_NODE("bottom_static", "bottomStaticOpt")
  UNIFORM_NODE("scan_line", "scalinesOpt")
  UNIFORM_NODE("rgb_offset", "rgbOffsetOpt")
  UNIFORM_NODE("horizontal_fuzz", "horzFuzzOpt")

  m_label.loadFont("verdana.ttf", 24);
  m_label.set_color(ofFloatColor::aqua);
  m_label.set_scale(0.5);
  m_label.set_position(ofVec2f(20., 20.));
  m_label.set_text("MORTON FELDMAN");
}

void biomedical_display::update()
{
  for(auto& osc : m_oscillos)
  {
    // osc->set_value(ofRandomf());
    osc->update();
  }
}

void biomedical_display::draw()
{
  ofLogNotice() << "draw";
  m_fbo.begin();
  ofClear(ofColor::black);
  int i=0;
  for(auto& osc : m_oscillos)
  {
    osc->draw(0.,float(i)/m_oscillos.size()*render_size.y,
             render_size.x,float(render_size.y)/m_oscillos.size());
    i++;
    m_label.draw();
  }
  m_fbo.end();

  m_PALfbo.begin();
  m_shader.begin();  
  m_shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
  m_shader.setUniform1f("iTime", ofGetElapsedTimef());
  m_shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 4.0f);

  m_shader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
  m_shader.setUniformTexture("iChannel0", m_fbo.getTexture(), m_fbo.getTexture().texData.textureID );
  {
    for(const auto& key : m_uniform_map)
      m_shader.setUniform1f(key.second.first, key.second.second);
    int offset = m_PALfbo.getWidth()-m_PALfbo.getHeight()/m_oscillos.size();
    m_fbo.draw(0.,0., offset, m_PALfbo.getHeight());
  }
  m_shader.end();
  m_PALfbo.end();

  m_PALfbo.draw(0,0,ofGetWidth(), ofGetHeight());
}

void biomedical_display::exit()
{
}

void biomedical_display::messageReceived(ofMessage& message)
{
}

void biomedical_display::windowResized   (ofResizeEventArgs& args)
{
  render_size.y = 576;
  render_size.x = float(ofGetWidth()*render_size.y)/ofGetHeight();

  m_fbo.allocate(render_size.x, render_size.y);
  m_fbo.begin();
  ofClear(ofColor::black);
  m_fbo.end();

  m_PALfbo.allocate(render_size.x, render_size.y);
  m_PALfbo.begin();
  ofClear(ofColor::black);
  m_PALfbo.end();

  m_shader.setUniformTexture("tex0", m_fbo.getTexture(),m_fbo.getTexture().texData.textureID);
}

