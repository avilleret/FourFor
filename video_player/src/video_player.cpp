#include "video_player.h"
#include <ossia/network/base/node.hpp>
#include <ossia/network/base/parameter.hpp>

video_player::video_player(const std::string& name)
  : m_server(name, 1236, 5680)
  , m_ecg(m_server.get_root_node().create_void("ecg"))
  , m_clock(m_server.get_root_node().create_void("clock"))
{
}

void init_fbo(ofFbo& fbo)
{
  fbo.allocate(1280, 720);

  fbo.begin();
  ofClear(0);
  fbo.end();
}

void video_player::setup()
{
  ofHideCursor();
  ofClear(ofColor::black);
  ofSetColor(ofColor::white);

#if OMXPLAYER
  {
    auto root = m_server.get_root_node().create_void("video");

    {
      auto n = root.create_bool("play");
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto b = v.to_bool();
        vp->lock();
        vp->setPaused(!b);
        vp->unlock();
      }, &m_player);
    }
    {
      auto n = root.create_string("load");
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto s = v.to_string();
        vp->lock();
        vp->m_file_path = ofToDataPath(std::string("videos/"+s),true);
        vp->m_file_path_changed = true;
        vp->unlock();
      }, &m_player);
    }
    {
      auto n = root.create_float("volume");
      n.set_min(0.);
      n.set_max(1.);
      n.set_bounding(opp::bounding_mode::Clip);
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto f = v.to_float();
        vp->lock();
        vp->setVolume(f);
        vp->unlock();
      }, &m_player);
    }
    {
      auto n = root.create_bool("loop");
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto b = v.to_bool();
        vp->lock();
        if(b)
          vp->enableLooping();
        else
          vp->disableLooping();
        vp->unlock();
      }, &m_player);
    }
    {
      auto n = root.create_bool("enable");
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto b = v.to_bool();
        vp->m_enable = b;
      }, &m_player);
      n.set_value(true);
    }
    {
      auto n = root.create_rgba("color");
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto vec = v.to_vec4f();
        vp->m_color = ofFloatColor(vec[0], vec[1], vec[2], vec[3]);
      }, &m_player);
      n.set_value(opp::value::vec4f{1.,1,1.,1.});
    }
    {
      auto n = root.create_vec2f("position");
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto vec = v.to_vec2f();
        vp->lock();
        vp->m_position = ofVec2f(vec[0], vec[1]);
        vp->unlock();
      }, &m_player);
      n.set_value(opp::value::vec2f{0.,0.});
    }

  }
#endif

  {
    for(int i=0; i<NUM_IMG; i++)
    {
      auto root = m_server.get_root_node().create_void("image.0");
      {
        auto n = root.create_string("load");
        n.set_value_callback(
              [](void* context, const opp::value& v){
          safe_image* im = static_cast<safe_image*>(context);
          auto s = v.to_string();
          im->lock();
          im->load(std::string("images/" + s));
          ofLogNotice() << "load " << s;
          im->changed = true;
          im->m_enable=true;
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
    }
  }
  auto n = m_server.get_root_node().find_child("image.0");
  n.find_child("load").set_value("sop1.png");
  n.find_child("scale").set_value(0.2);

  /*
  n = m_server.get_root_node().find_child("image.1").find_child("load");
  n.set_value("sop1.png");

  n = m_server.get_root_node().find_child("image.2").find_child("load");
  n.set_value("sop2.png");

  n = m_server.get_root_node().find_child("image.3").find_child("load");
  n.set_value("sop3.png");
  */

#if OMXPLAYER
  m_player.setVolume(0.);
#endif

  if(!m_shader.load("shaders/feedback"))
  {
    ofLogError("FourFor") << "Can't link shader, abord";
    ofExit();
  }
  else
  {
    auto map = m_shader.getActiveUniforms();
    auto n = m_server.get_root_node().create_void("shader");
    for(const auto& p : map)
    {
      ofLogNotice() << "found uniform " << p.first << " type " << p.second;

      opp::node node;
      if(p.first == "modelViewProjectionMatrix"
         || p.first == "tex1"
         || p.first == "tex0"
         || p.first == "resolution"
         || p.first == "time")
        continue;

      switch(p.second)
      {
        case GL_BOOL:
          node = n.create_bool(p.first);
          break;
        case GL_FLOAT:
          ofLogNotice() << "make float parameter";
          node = n.create_float(p.first);
          break;
        case GL_INT:
          node = n.create_int(p.first);
          break;
        case GL_BOOL_VEC2:
        case GL_INT_VEC2:
        case GL_FLOAT_VEC2:
          node = n.create_vec2f(p.first);
          break;
        case GL_BOOL_VEC3:
        case GL_INT_VEC3:
        case GL_FLOAT_VEC3:
          node = n.create_vec3f(p.first);
          break;
        case GL_BOOL_VEC4:
        case GL_INT_VEC4:
        case GL_FLOAT_VEC4:
          node = n.create_vec4f(p.first);
          break;
        default:
          ofLogVerbose() << "uniform parameter " << p.first << " has unsupported type: " << p.second;
      }

      auto node_ptr = node.get_raw_node_pointer();
      if(!node_ptr)
        continue;

      auto param = node_ptr->get_parameter();
      if(!param)
        continue;

      param->add_callback( [=](const ossia::value& v)
      {
         m_uniform_mutex.lock();
         m_uniforms_map[p.first] = opp::value(v);
         m_uniform_mutex.unlock();
      });
    }
  }

  init_fbo(m_draw_fbo);
  init_fbo(m_prev);
  init_fbo(m_curr);


  m_server.get_root_node().load_preset(m_server.get_name() + ".txt");

  auto node=m_server.get_root_node().create_void("preset");
  {
    auto n = node.create_impulse("save");
    n.set_value_callback([](void* ctx, const opp::value&)
    {
      video_player* ptr = static_cast<video_player*>(ctx);
      std::string filename = ofToDataPath(ptr->m_server.get_name() + ".txt", true);
      ofLogNotice() << "saving preset to " << filename;
      ptr->m_server.get_root_node().save_preset(filename);
    }, this);
  }
  {
    auto n = node.create_impulse("load");
    n.set_value_callback([](void* ctx, const opp::value&)
    {
      video_player* ptr = static_cast<video_player*>(ctx);
      std::string filename = ptr->m_server.get_name() + ".txt";
      ptr->m_server.get_root_node().load_preset(ofToDataPath(filename, true));
    }, this);
  }
}

void video_player::update()
{
  m_player.update();
  // we need to update image in the main thread
  for(int i=0; i<NUM_IMG; i++)
    if(m_images[i].changed)
    {
      ofLogNotice() << "update image " << i;
      m_images[i].update();
      m_images[i].changed = false;
    }

  m_ecg.update();
}

void video_player::draw()
{
  m_draw_fbo.begin();
  ofClear(0,0,0,255);
  ofSetColor(ofColor::white);

#if OMXPLAYER
  if(m_player.isPlaying())
    m_player.draw_safe(m_draw_fbo.getWidth(), m_draw_fbo.getHeight());
#endif

  for(int i = 0; i<NUM_IMG; i++)
    if(m_images[i].isAllocated())
      m_images[i].draw_safe();

  m_ecg.draw(0,0,m_draw_fbo.getWidth(), m_draw_fbo.getHeight());
  m_clock.draw();
  m_draw_fbo.end();

  m_curr.begin();
  m_shader.begin();
  m_shader.setUniformTexture("tex0", m_draw_fbo.getTexture(), m_draw_fbo.getTexture().getTextureData().textureID);
  m_shader.setUniformTexture("tex1", m_prev.getTexture(), m_prev.getTexture().getTextureData().textureID);
  m_shader.setUniform1f("time", ofGetElapsedTimef());
  m_shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());

  auto node = m_server.get_root_node().find_child("shader");
  m_uniform_mutex.lock();
  for(const auto u : m_uniforms_map)
  {
    auto node_name = u.first;
    auto n = node.find_child(u.first);
    auto ptr = n.get_raw_node_pointer();

    if(!ptr)
    {
      ofLogError() << "invalid pointer on node: " << u.first;
      continue;
    }

    auto p = ptr->get_parameter();
    if(!p)
    {
      ofLogError() << "no parameter on this node: " << u.first;
    }
    // TODO WTF is this line ?
    switch (n.get_raw_node_pointer()->get_parameter()->get_value_type())
    {
      case ossia::val_type::FLOAT:
         m_shader.setUniform1f(u.first, u.second.to_float());
        break;
      case ossia::val_type::INT:
        m_shader.setUniform1f(u.first, u.second.to_int());
        break;
      case ossia::val_type::BOOL:
        m_shader.setUniform1f(u.first, u.second.to_bool());
        break;
      case ossia::val_type::VEC2F:
        m_shader.setUniform2f(u.first, u.second.to_vec2f()[0], u.second.to_vec2f()[1]);
        break;
      case ossia::val_type::VEC3F:
        m_shader.setUniform3f(u.first, u.second.to_vec2f()[0],
            u.second.to_vec2f()[1], u.second.to_vec2f()[2]);
        break;
      case ossia::val_type::VEC4F:
        m_shader.setUniform4f(u.first, u.second.to_vec2f()[0],
            u.second.to_vec2f()[1], u.second.to_vec2f()[2],
            u.second.to_vec2f()[3]);
        break;
     default:
        ;
    }
  }
  m_uniform_mutex.unlock();
//  m_shader.setUniform1f("freq", 2.0);
//  m_shader.setUniform1f("amplitude", 1.0);
//  float alpha = 0.f;
//  m_shader.setUniform1f("alpha", alpha);
//  m_shader.setUniform1f("beta", .9f);
//  m_shader.setUniform2f("anchor", .5f, .5f);
  m_draw_fbo.draw(0.,0., m_curr.getWidth(), m_curr.getHeight());
  m_shader.end();
  m_curr.end();

  m_curr.draw(0,0,ofGetWidth(), ofGetHeight());

  swap(m_curr, m_prev);
}

void video_player::exit()
{
}

void video_player::windowResized(ofResizeEventArgs&)
{
}

void video_player::messageReceived(ofMessage& message)
{
}
