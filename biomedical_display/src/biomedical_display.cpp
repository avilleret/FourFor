#include "biomedical_display.h"

biomedical_display::biomedical_display()
  : m_server("ecg", 1234, 5678)
  , m_shader(m_server.get_root_node().create_void("shader"))
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

  ofResizeEventArgs size(ofGetWidth(), ofGetHeight());
  windowResized(size);

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
  m_draw_fbo.begin();
  ofClear(ofColor::black);
  int i=0;
  for(auto& osc : m_oscillos)
  {
    osc->draw(0.,float(i)/m_oscillos.size()*render_size.y,
             render_size.x,float(render_size.y)/m_oscillos.size());
    i++;
    m_label.draw();
  }
  m_draw_fbo.end();

  m_curr.begin();
  {
    m_shader.begin();
    m_shader.setUniformTexture("tex0", m_draw_fbo.getTexture(), m_draw_fbo.getTexture().getTextureData().textureID);
    m_shader.setUniformTexture("tex1", m_prev.getTexture(), m_prev.getTexture().getTextureData().textureID);
    {
      m_draw_fbo.draw(0.,0., m_curr.getWidth(), m_curr.getHeight());
    }
    m_shader.end();
  }
  m_curr.end();

  m_curr.draw(0,0,ofGetWidth(), ofGetHeight());

  swap(m_prev, m_curr);
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

  for(ofFbo* fbo : {&m_curr, &m_prev, &m_draw_fbo, &m_draw_fbo})
  {
    fbo->allocate(render_size.x, render_size.y);
    fbo->begin();
      ofClear(ofColor::black);
    fbo->end();
  }
}

