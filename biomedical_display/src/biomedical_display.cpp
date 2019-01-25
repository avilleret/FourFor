#include "biomedical_display.h"

void biomedical_display::setup()
{
  ofSetBackgroundColor(0);

  m_server.setup("biomedical_display");
  auto root = m_server.get_root_node();

  for(int i=0; i<3; i++)
  {
    m_oscillos.push_back(Oscilloscope(root.create_void("oscillo.1")));
  }

  m_oscillos[0].set_color(ofColor::green);
  m_oscillos[1].set_color(ofColor::blue);
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
  int i=0;
  for(auto& osc : m_oscillos)
  {
    osc.draw(0.,float(i)/m_oscillos.size(),1.,1./m_oscillos.size());
    i++;
  }
}

void biomedical_display::exit()
{
}

void biomedical_display::messageReceived(ofMessage& message)
{
}
