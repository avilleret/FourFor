#include "video_player.h"

video_player::video_player()
  : m_server("video_player", 1236, 5680)
  , m_clock(m_server.get_root_node().create_void("clock"))
{

}

void video_player::setup()
{
  ofHideCursor();
  ofClear(ofColor::black);
  ofSetColor(ofColor::white);

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
        vp->loadMovie(ofToDataPath(std::string("videos/"+s),true));
        vp->m_enable=true;
        vp->start();
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
      auto n = root.create_bool("enable");
      n.set_value_callback(
            [](void* context, const opp::value& v){
        SafePlayer* vp = static_cast<SafePlayer*>(context);
        auto b = v.to_bool();
        vp->m_enable = b;
      }, &m_player);
    }
  }

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
  auto n = m_server.get_root_node().find_child("image.0").find_child("load");
  n.set_value("sop0.png");

  n = m_server.get_root_node().find_child("image.1").find_child("load");
  n.set_value("sop1.png");

  n = m_server.get_root_node().find_child("image.2").find_child("load");
  n.set_value("sop2.png");

  n = m_server.get_root_node().find_child("image.3").find_child("load");
  n.set_value("sop3.png");

}

void video_player::update()
{
  // we need to update image in the main thread
  for(int i=0; i<NUM_IMG; i++)
    if(m_images[i].changed)
    {
      ofLogNotice() << "update image " << i;
      m_images[i].update();
      m_images[i].changed = false;
    }
}

void video_player::draw()
{
  ofClear(0);

  for(int i = 0; i<NUM_IMG; i++)
    if(m_images[i].isAllocated())
      m_images[i].draw_safe();

  m_clock.draw();

  // m_player.draw_safe();
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
