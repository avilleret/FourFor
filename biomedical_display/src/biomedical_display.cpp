#include "biomedical_display.h"

#define UNIFORM_NODE(node_name, uniform_name) \
{ \
  auto root = m_server.get_root_node(); \
  auto n = root.create_float(node_name); \
  n.set_value_callback([](void* ctx, const opp::value& v){ \
    biomedical_display* ptr = static_cast<biomedical_display*>(ctx); \
    float f = v.to_float(); \
    ptr->get_shader().setUniform1f(uniform_name, f); \
    std::cout << "update " << uniform_name << " to " << f << std::endl; \
  }, this); \
  n.set_value(0); \
}

void biomedical_display::setup()
{
  ofSetBackgroundColor(ofColor::black);

  m_server.setup("biomedical_display");
  auto root = m_server.get_root_node();

  for(int i=0; i<3; i++)
  {
    m_oscillos.push_back(Oscilloscope(root.create_void("oscillo.1")));
  }

  m_oscillos[0].set_color(ofColor::green);
  m_oscillos[1].set_color(ofColor::blue);

  m_shader.load("shaders/distorted TV.fs");
  m_shader.setAdvanceTime(true);

  UNIFORM_NODE("vertical_jerk","vertJerkOpt")
  UNIFORM_NODE("vertical_movement", "vertMovementOpt")
  UNIFORM_NODE("bottom_static", "bottomStaticOpt")
  UNIFORM_NODE("scan_line", "scalinesOpt")
  UNIFORM_NODE("rgb_offset", "rgbOffsetOpt")
  UNIFORM_NODE("horizontal_fuzz", "horzFuzzOpt")
}

void biomedical_display::update()
{
  for(auto& osc : m_oscillos)
  {
    // osc.set_value(ofRandomf());
    osc.update();
  }
}

void biomedical_display::draw()
{
  m_shader.begin();
  int i=0;
  for(auto& osc : m_oscillos)
  {
    osc.draw(0.,float(i)/m_oscillos.size(),1.,1./m_oscillos.size());
    i++;
  }
  m_shader.end();
}

void biomedical_display::exit()
{
}

void biomedical_display::messageReceived(ofMessage& message)
{
}

void biomedical_display::windowResized   (ofResizeEventArgs& args)
{
  m_shader.setDimensions(args.width, args.height);
}

