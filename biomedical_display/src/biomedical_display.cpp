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

  {
    auto& txt = m_texts.add(m_server.get_root_node().create_void("name"));
    txt.set_color(ofFloatColor::aqua);
    txt.set_scale(0.2);
    txt.set_position(ofVec2f(20., 40.));
    txt.set_text("FELDMAN Morton\t\t\t26.01.12");
  }

  int i=0;
  vector<pair<string, string>> txts = {{"83.4", "bpm"}, {"22.4", "cpm"}, {"92%", "SpO2"}};
  for(auto& c : {ofColor::green, ofColor::yellow, ofColor::aqua})
  {
    {
      auto& txt = m_texts.add(m_server.get_root_node().create_void("number.1"));
      txt.set_color(c);
      txt.set_scale(0.8);
      txt.set_position(ofVec2f(render_size.x-250., 120.+i*render_size.y/m_oscillos.size()));
      txt.set_text(txts[i].first);
    }

    {
      auto& txt = m_texts.add(m_server.get_root_node().create_void("label.1"));
      txt.set_color(c);
      txt.set_scale(0.2);
      txt.set_position(ofVec2f(render_size.x-80., 150.+i*render_size.y/m_oscillos.size()));
      txt.set_text(txts[i].second);
    }
    i++;
  }

  {
    for(int i=0; i<NUM_IMG; i++)
    {
      auto root = m_server.get_root_node().create_void("image.1");
      {
        auto n = root.create_string("load");
        n.set_value_callback(
              [](void* context, const opp::value& v){
          safe_image* im = static_cast<safe_image*>(context);
          auto s = v.to_string();
          im->lock();
          im->m_file = s;
          ofLogNotice() << "try to load " << s;
          im->m_changed = true;
          im->unlock();
        }, &m_images[i]);
      }
      {
        auto n = root.create_vec2f("position");
        n.set_value_callback(
              [](void* context, const opp::value& v){
          safe_image* im = static_cast<safe_image*>(context);
          auto vec = v.to_vec2f();
          im->lock();
          im->m_position=vec;
          im->unlock();
        }, &m_images[i]);
      }
      {
        auto n = root.create_float("scale");
        n.set_value(1.);
        n.set_value_callback(
              [](void* context, const opp::value& v){
          safe_image* im = static_cast<safe_image*>(context);
          auto f = v.to_float();
          im->lock();
          im->m_scale=f;
          im->unlock();
        }, &m_images[i]);
      }
      {
        auto n = root.create_bool("enable");
        n.set_value_callback(
              [](void* context, const opp::value& v){
          safe_image* im = static_cast<safe_image*>(context);
          auto b = v.to_bool();
          im->m_enable = b;
        }, &m_images[i]);
      }
      {
        auto n = root.create_rgba("color");
        n.set_value_callback(
              [](void* context, const opp::value& v){
          safe_image* im = static_cast<safe_image*>(context);
          auto vec = v.to_vec4f();
          im->lock();
          im->m_color = ofFloatColor(vec[0], vec[1], vec[2], vec[3]);
          im->unlock();
        },  &m_images[i]);
        n.set_value(opp::value::vec4f{1.,1,1.,1.});
      }
    }
  }
}

void biomedical_display::update()
{
  for(auto& osc : m_oscillos)
  {
    // osc->set_value(ofRandomf());
    osc->update();
  }

  for(int i=0; i<NUM_IMG; i++)
    m_images[i].update();
}

void biomedical_display::draw()
{
  m_draw_fbo.begin();
  {
    ofClear(ofColor::black);
    int i=0;
    for(auto& osc : m_oscillos)
    {
      osc->draw(0.,float(i)/m_oscillos.size()*render_size.y,
                render_size.x,float(render_size.y)/m_oscillos.size());
      i++;
    }
    m_texts.draw();
    for(int i = 0; i<NUM_IMG; i++)
      if(m_images[i].isAllocated())
        m_images[i].draw_safe();
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

