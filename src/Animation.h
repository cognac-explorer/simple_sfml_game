#pragma once

#include <string>
#include "Vec2.h"
#include <vector>
#include "raylib.h"


class Animation
{
    Texture2D   m_texture;
    Rectangle   m_frameRect;
    Vec2        m_origin;
    size_t      m_frameCount = 1;
    size_t      m_currentFrame = 0;
    size_t      m_speed;
    Vec2        m_size = {1, 1};
    std::string m_name = "none";

public:

    Animation();
    Animation(const std::string& name, const Texture2D& t);
    Animation(const std::string& name, const Texture2D& t, size_t frameCount, size_t speed);

    void update();
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2& getSize() const;
    void draw(const Vec2& position, const Vec2& scale = {1.0f, 1.0f}, float rotation = 0.0f, bool facingLeft = false);
};
