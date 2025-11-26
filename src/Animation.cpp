#include "Animation.h"
#include <cmath>
#include <iostream>


Animation::Animation()
{

}

Animation::Animation(const std::string& name, const Texture2D& t)
    : m_name(name)
    , m_texture(t)
    , m_speed(0)
{
    m_size = Vec2((float)t.width, (float)t.height);
    m_frameRect = { 0.0f, 0.0f, m_size.x, m_size.y };
    m_origin = Vec2(m_size.x / 2.0f, m_size.y / 2.0f);
}

Animation::Animation(const std::string& name, const Texture2D& t, size_t frameCount, size_t speed)
    : m_name(name)
    , m_texture(t)
    , m_frameCount(frameCount)
    , m_speed(speed)
{
    m_size = Vec2((float)t.width / frameCount, (float)t.height);
    m_frameRect = { 
        m_currentFrame * m_size.x,
        0.0f,
        m_size.x,
        m_size.y
    };
    m_origin = Vec2(m_size.x / 2.0f, m_size.y / 2.0f);
}

void Animation::update()
{   
    if (m_frameCount == 1)
    {
        m_frameRect = { 0.0f, 0.0f, m_size.x, m_size.y };
    }
    else
    {
        if (m_currentFrame % m_speed == 0)
        {
            float frameIndex = std::floor(m_currentFrame / m_speed);
            m_frameRect = { 
                frameIndex * m_size.x, 
                0.0f, 
                m_size.x, 
                m_size.y 
            };
        }
        m_currentFrame++;
    }
}

const Vec2& Animation::getSize() const
{
    return m_size;
}

const std::string& Animation::getName() const
{
    return m_name;
}

void Animation::draw(const Vec2& position, const Vec2& scale, float rotation, bool facingLeft)
{
    // raylib way for doing horizontal flip
    Rectangle source = m_frameRect;
    if (facingLeft)
    {
        source.width = -source.width;
    }

    Rectangle dest = {
        position.x,
        position.y,
        m_size.x * scale.x,
        m_size.y * scale.y
    };
    
    DrawTexturePro(m_texture, source, dest, m_origin, rotation, WHITE);
}

bool Animation::hasEnded() const
{
    if (m_speed != 0 && m_currentFrame / m_speed == m_frameCount)
    {
        return true;
    }
    return false;
}
