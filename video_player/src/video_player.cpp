#include "video_player.h"

#ifdef WIRINGPI
#include <wiringPi.h>
#endif

video_player::video_player(const std::string& name)
  : m_server(name, 1236, 5680)
  , m_shader(m_server.get_root_node().create_void("shader"))
  , m_sd(name == "tv")
#ifdef OMXPLAYER
  , m_player(m_sd)
#endif
{
  // m_clock.add(m_server.get_root_node().create_void("clock"));
}

void video_player::init_fbo(ofFbo& fbo)
{
  if(m_sd)
   fbo.allocate(ofGetWidth(), ofGetHeight());
  else
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
        if(!b)
          vp->m_file_path_changed = false;
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
      m_images[i].m_sd = m_sd;
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

#ifdef WIRINGPI
#define RELAY 7
  {
    wiringPiSetup () ;
    pinMode (RELAY, OUTPUT) ;

    auto n = m_server.get_root_node().create_bool("relay");
    n.set_value_callback(
          [](void*, const opp::value& v){
      auto b = v.to_bool();
      digitalWrite(RELAY, b);
    }, nullptr);
    n.set_critical(true);
  }
#endif
//  auto n = m_server.get_root_node().find_child("image.1");
//  n.find_child("load").set_value("sop1.png");
//  n.find_child("scale").set_value(0.2);

  /*
  n = m_server.get_root_node().find_child("image.1").find_child("load");
  n.set_value("sop1.png");

  n = m_server.get_root_node().find_child("image.2").find_child("load");
  n.set_value("sop2.png");

  n = m_server.get_root_node().find_child("image.3").find_child("load");
  n.set_value("sop3.png");
  */

  init_fbo(m_draw_fbo);
  init_fbo(m_prev);
  init_fbo(m_curr);
  ofLogNotice() << "fbo resolution: " << m_draw_fbo.getWidth() << "x" << m_draw_fbo.getHeight();

  // m_server.get_root_node().load_preset(m_server.get_name() + ".txt");
/*
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
*/
}

void video_player::update()
{
#ifdef OMXPLAYER
  m_player.update();
#endif
  // we need to update image in the main thread
  for(int i=0; i<NUM_IMG; i++)
    m_images[i].update();
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

  // m_ecg.draw(0,0,m_draw_fbo.getWidth(), m_draw_fbo.getHeight());
  //m_clock.draw();
  m_draw_fbo.end();

  m_curr.begin();
  m_shader.begin();
  m_shader.setUniformTexture("tex0", m_draw_fbo.getTexture(), m_draw_fbo.getTexture().getTextureData().textureID);
  m_shader.setUniformTexture("tex1", m_prev.getTexture(), m_prev.getTexture().getTextureData().textureID);
  m_draw_fbo.draw(0.,0., m_curr.getWidth(), m_curr.getHeight());
  m_shader.end();
  m_curr.end();

  m_curr.draw(0,0,ofGetWidth(), ofGetHeight());
  // m_prev.draw(0,0,200,200); // for debugging

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
