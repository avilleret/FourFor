#pragma once

#include "ofMain.h"
#include <ossia-cpp/ossia-cpp98.hpp>

class AbstractText : private std::mutex
{
  public:
    AbstractText(const opp::node& root);
    void set_text(const std::string& s);
    void set_position(const ofVec2f& pos);
    void set_scale(float f);
    void set_color(const ofFloatColor& c);

    // TODO add other getters
    std::string get_text() const { return m_text; }
    std::string get_name() const { return m_root.get_name(); }

    void begin();
    void end();

  protected:
    std::string m_text;

  private:
    opp::node m_root;
    ofVec2f m_position;
    float m_scale{1.};
    ofFloatColor m_color{ofFloatColor::white};
};

class OssiaText : public ofTrueTypeFont, public AbstractText
{
  public:
    OssiaText(const opp::node& root);
    void draw();
};

class OssiaMultiText : public ofTrueTypeFont
{
  public:
    OssiaMultiText();
    AbstractText& add(const opp::node& root);
    void draw();
    OssiaText& get_ossia_text(const std::string& name);

  private:
    std::vector<std::shared_ptr<AbstractText>> m_texts;
};
