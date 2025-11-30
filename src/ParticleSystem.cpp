#include <vector>
#include <iostream>
#include "Vec2.h"
#include "rlgl.h"


class ParticleSystem
{
    Vec2 m_centerPosition;

    struct Particle
    {
        Vec2 velocity;
        int lifetime;
    };

    Color color = MAGENTA;
    float m_size = 8.0f;

    std::vector<Particle> m_particles;
    std::vector<Vec2> m_vertices;

    void resetParticles(size_t count = 1000)
    {
        m_particles = std::vector<Particle>(count);
        m_vertices = std::vector<Vec2>(count * 4);

        for(size_t index = 0; index < count; index++)
        {
            resetParticle(index);
        }
    };

    void resetParticle(size_t index)
    {
        float centerX = m_centerPosition.x;
        float centerY = m_centerPosition.y;
        float halfSize = m_size / 2.0f;

        // Define the quad centered around (centerX, centerY)
        m_vertices[4*index + 0] = Vec2(centerX - halfSize, centerY - halfSize); // Top-Left
        m_vertices[4*index + 1] = Vec2(centerX + halfSize, centerY - halfSize); // Top-Right
        m_vertices[4*index + 2] = Vec2(centerX + halfSize, centerY + halfSize); // Bottom-Right
        m_vertices[4*index + 3] = Vec2(centerX - halfSize, centerY + halfSize); // Bottom-Left

        float rx = ((float)rand() / RAND_MAX) * 10 - 5;
        float ry = ((float)rand() / RAND_MAX) * 10 - 5;

        m_particles[index].velocity = { rx, ry };
        m_particles[index].lifetime = 60 + rand() % 60;
    };

public:

    void init(Vec2 pos)
    {
        m_centerPosition = pos;
        resetParticles();
    }

    void update()
    {
        for(size_t index = 0; index < m_particles.size(); ++index)
        {
            if(m_particles[index].lifetime == 0)
            {
                resetParticle(index);
            }

            m_vertices[4*index + 0] += m_particles[index].velocity;
            m_vertices[4*index + 1] += m_particles[index].velocity;
            m_vertices[4*index + 2] += m_particles[index].velocity;
            m_vertices[4*index + 3] += m_particles[index].velocity;

            m_particles[index].lifetime--;
        }
    }

/*
    NOTE: DrawRectangle() performs equally well here due to Raylib's automatic batching.
    Manual batching with rlgl probably becomes faster when:
    - Each particle needs different colors
    - Drawing more complex shapes
*/
    void draw() const
    {
        rlDisableBackfaceCulling();  // crucial, have something to do with vertices sequence
        rlBegin(RL_TRIANGLES);  // quads doesn't works

        for (size_t i = 0; i < m_particles.size(); ++i)
        {
            // Get the 4 vertices for this particle
            Vec2 v0 = m_vertices[4*i + 0]; // Top-Left
            Vec2 v1 = m_vertices[4*i + 1]; // Top-Right
            Vec2 v2 = m_vertices[4*i + 2]; // Bottom-Right
            Vec2 v3 = m_vertices[4*i + 3]; // Bottom-Left
            
            // First triangle (Top-Left, Top-Right, Bottom-Left)
            rlColor4ub(color.r, color.g, color.b, 255);
            rlVertex2f(v0.x, v0.y);
            rlColor4ub(color.r, color.g, color.b, 255);
            rlVertex2f(v1.x, v1.y);
            rlColor4ub(color.r, color.g, color.b, 255);
            rlVertex2f(v3.x, v3.y);
            
            // Second triangle (Top-Right, Bottom-Right, Bottom-Left)
            rlColor4ub(color.r, color.g, color.b, 255);
            rlVertex2f(v1.x, v1.y);
            rlColor4ub(color.r, color.g, color.b, 255);
            rlVertex2f(v2.x, v2.y);
            rlColor4ub(color.r, color.g, color.b, 255);
            rlVertex2f(v3.x, v3.y);
        }
        
        rlEnd();
    }
};
