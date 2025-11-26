#include <iostream>
#include <fstream>
#include <sstream>

#include "Assets.h"

void Assets::loadFromFile(const std::string path)
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
            Texture2D& t = getTexture(texture_name);
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

void Assets::addTexture(const std::string& name, const std::string& path)
{
    Texture2D texture = LoadTexture(path.c_str());
    if(texture.id == 0)
    {
        std::cerr << "Error while loading texture: " << path << std::endl;
        return;
    }
    m_textures[name] = texture;
}

Texture2D& Assets::getTexture(const std::string& name)
{
    return m_textures.at(name);
}

void Assets::addAnimation(const std::string& name, const Animation& animation)
{
    m_animations[name] = animation;
}

const Animation& Assets::getAnimation(const std::string& name) const
{
    return m_animations.at(name);
}

void Assets::addFont(const std::string& name, const std::string& path)
{
    Font font = LoadFont(path.c_str());
    if(font.texture.id == 0)
    {
        std::cerr << "Error opening font file " << path << std::endl;
    }
    m_fonts[name] = font;
}

const Font& Assets::getFont(const std::string& name) const
{
    return m_fonts.at(name);
}

void Assets::clear()
{
    for(auto& [name, texture] : m_textures)
    {
        UnloadTexture(texture);
    }
    
    for(auto& [name, font] : m_fonts)
    {
        UnloadFont(font);
    }
    
    m_textures.clear();
    m_fonts.clear();
}
