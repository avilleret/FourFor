#include "OssiaShader.h"
#include <ossia/network/base/node.hpp>
#include <ossia/network/base/parameter.hpp>

OssiaShader::OssiaShader(const opp::node& root)
  : m_root(root)
{

}

void OssiaShader::load(const std::string& file)
{
  if(!ofShader::load(file))
    {
      ofLogError("FourFor") << "Can't link shader, abord";
      ofExit();
    }
    else
    {
      auto map = getActiveUniforms();
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
            node = m_root.create_bool(p.first);
            break;
          case GL_FLOAT:
            ofLogNotice() << "make float parameter";
            node = m_root.create_float(p.first);
            break;
          case GL_INT:
            node = m_root.create_int(p.first);
            break;
          case GL_BOOL_VEC2:
          case GL_INT_VEC2:
          case GL_FLOAT_VEC2:
            node = m_root.create_vec2f(p.first);
            break;
          case GL_BOOL_VEC3:
          case GL_INT_VEC3:
          case GL_FLOAT_VEC3:
            node = m_root.create_vec3f(p.first);
            break;
          case GL_BOOL_VEC4:
          case GL_INT_VEC4:
          case GL_FLOAT_VEC4:
            node = m_root.create_vec4f(p.first);
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
}

void OssiaShader::begin()
{
  ofShader::begin();
  setUniform1f("time", ofGetElapsedTimef());
  setUniform2f("resolution", ofGetWidth(), ofGetHeight());

  m_uniform_mutex.lock();
  for(const auto& u : m_uniforms_map)
  {
    auto node_name = u.first;
    auto n = m_root.find_child(u.first);
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
         setUniform1f(u.first, u.second.to_float());
        break;
      case ossia::val_type::INT:
        setUniform1f(u.first, u.second.to_int());
        break;
      case ossia::val_type::BOOL:
        setUniform1f(u.first, u.second.to_bool());
        break;
      case ossia::val_type::VEC2F:
        setUniform2f(u.first, u.second.to_vec2f()[0], u.second.to_vec2f()[1]);
        break;
      case ossia::val_type::VEC3F:
        setUniform3f(u.first, u.second.to_vec2f()[0],
            u.second.to_vec2f()[1], u.second.to_vec2f()[2]);
        break;
      case ossia::val_type::VEC4F:
        setUniform4f(u.first, u.second.to_vec2f()[0],
            u.second.to_vec2f()[1], u.second.to_vec2f()[2],
            u.second.to_vec2f()[3]);
        break;
     default:
        ;
    }
  }
  m_uniform_mutex.unlock();
}
