#include <iostream>
#include <fstream>
#include <sstream>

#include "Assets.h"

void Assets::loadFromFile(std::string path)
{
    std::ifstream inputFile(path);

    if (!inputFile)
    {
        std::cerr << "Error opening file example.txt" << std::endl;
    }

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string asset;
        iss >> asset;
        if (asset == "Texture")
        {
            std::string name, path;
            iss >> name >> path;
            addTexture(name, path);
        }
        if (asset == "Animation")
        {
            std::string name, texture_name;
            int frames, speed;
            iss >> name >> texture_name >> frames >> speed;
            sf::Texture & t = getTexture(texture_name);
            Animation a = Animation(name, t, frames, speed);
            addAnimation(name, a);
        }
        if (asset == "Font")
        {
            std::string name, path;
            iss >> name >> path;
            addFont(name, path);
        }
    }
    
    inputFile.close();
    // std::cout << "Size of the textures map: " << m_textures.size() << std::endl;
    // std::cout << "Size of the animation map: " << m_animations.size() << std::endl;

}

void Assets::addTexture(std::string & name, std::string & path)
{
    sf::Texture texture;
    if(!texture.loadFromFile(path))
    {
        std::cerr << "Error while loading texture!" << std::endl;
    }
    m_textures[name] = texture;
}

sf::Texture & Assets::getTexture(const std::string & name)
{
    return m_textures.at(name);
}

void Assets::addAnimation(std::string & name, Animation & animation)
{
    m_animations[name] = animation;
}

const Animation & Assets::getAnimation(const std::string & name) const
{
    return m_animations.at(name);
}

void Assets::addFont(std::string & name, std::string & path)
{
    sf::Font font;
    if (!font.loadFromFile(path)) 
    {
        std::cerr << "Error opening font file " << path << std::endl;
    }
    m_fonts[name] = font;
}

const sf::Font & Assets::getFont(const std::string & name) const
{
    return m_fonts.at(name);
}
