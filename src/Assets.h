#pragma once

#include <map>
#include "Animation.h"

class Assets
{
    std::map<std::string, Texture2D> m_textures;
    std::map<std::string, Animation>   m_animations;
    std::map<std::string, Font>    m_fonts;
public:
    Assets() {};
    void loadFromFile(const std::string path);
    void addTexture(const std::string& name, const std::string& path);
    void addAnimation(const std::string& name, const Animation& animation);
    void addFont(const std::string& name, const std::string& path);
    void clear();

    Texture2D& getTexture(const std::string& name);
    const Animation& getAnimation(const std::string& name) const;
    const Font& getFont(const std::string& name) const;
};
