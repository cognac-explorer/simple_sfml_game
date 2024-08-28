#pragma once

#include <map>
#include "Animation.h"

class Assets
{
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, Animation>   m_animations;
    std::map<std::string, sf::Font>    m_fonts;
public:
    Assets() {};
    void loadFromFile(std::string path);
    void addTexture(std::string & name, std::string & path);
    void addAnimation(std::string & name, Animation & animation);
    void addFont(std::string & name, std::string & path);

    sf::Texture & getTexture(const std::string & name);
    const Animation & getAnimation(const std::string & name) const;
    const sf::Font & getFont(const std::string & name) const;
};