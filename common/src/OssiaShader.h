#pragma once

#include "ofMain.h"
#include <ossia-cpp/ossia-cpp98.hpp>

class OssiaShader : public ofShader
{
  public:
    OssiaShader(const opp::node& root);
    void load(const std::string&);
    void begin();

  private:
    opp::node m_root;

    std::unordered_map<std::string, opp::value> m_uniforms_map;
    std::mutex m_uniform_mutex;
};
