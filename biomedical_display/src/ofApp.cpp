#include "ofApp.h"

void ofApp::setup()
{
  ofSetBackgroundColor(0);
  m_oscillos.resize(3);

  m_oscillos[0].setColor(ofColor::green);
  m_oscillos[1].setColor(ofColor::blue);
}

void ofApp::update()
{
  for(auto& osc : m_oscillos)
    osc.update(ofRandomf());
}

void ofApp::draw()
{
  int i=0;
  for(auto& osc : m_oscillos)
  {
    osc.draw(0.,float(i)/m_oscillos.size(),1.,1./m_oscillos.size());
    i++;
  }
}

void ofApp::exit()
{
}

void ofApp::messageReceived(ofMessage& message)
{
}
