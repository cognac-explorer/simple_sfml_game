// #include <vector>
// #include <iostream>

// class ParticleSystem
// {
//     struct Particle
//     {
//         sf::Vector2f velocity;
//         int lifetime;
//     };

//     std::vector<Particle> m_particles;
//     sf::Vector2u m_windowSize;
//     sf::VertexArray m_vertices;
//     float m_size = 8;

//     void resetParticles(size_t count = 1024)
//     {
//         m_particles = std::vector<Particle>(count);
//         m_vertices = sf::VertexArray(sf::Quads, count * 4);
        
//         for(size_t index; index < count; index++)
//         {
//             resetParticle(index);
//         }

//     };

//     void resetParticle(size_t index)
//     {
//         float mx = m_windowSize.x / 2;
//         float my = m_windowSize.y / 2;

//         m_vertices[4*index + 0] = sf::Vector2f(mx, my);
//         m_vertices[4*index + 1] = sf::Vector2f(mx + m_size, my);
//         m_vertices[4*index + 2] = sf::Vector2f(mx + m_size, my + m_size);
//         m_vertices[4*index + 3] = sf::Vector2f(mx, my + m_size);
        
//         sf::Color color(255, 0, 255, 255);
//         m_vertices[4*index + 0].color = color;
//         m_vertices[4*index + 1].color = color;
//         m_vertices[4*index + 2].color = color;
//         m_vertices[4*index + 3].color = color;

//         float rx = ((float)rand() / RAND_MAX) * 10 - 5;
//         float ry = ((float)rand() / RAND_MAX) * 10 - 5;

//         m_particles[index].velocity = sf::Vector2f(rx, ry);

//         m_particles[index].lifetime = 60 + rand() % 60;
//     };

// public:

//     void init(sf::Vector2u windowSize)
//     {
//         m_windowSize = windowSize;
//         resetParticles();
//     }

//     void update()
//     {
//         // std::cout << "update" << std::endl;
//         for(size_t index = 0; index < m_particles.size(); ++index)
//         {
//             if(m_particles[index].lifetime == 0)
//             {
//                 resetParticle(index);
//             }
//             m_vertices[4*index + 0].position += m_particles[index].velocity;
//             m_vertices[4*index + 1].position += m_particles[index].velocity;
//             m_vertices[4*index + 2].position += m_particles[index].velocity;
//             m_vertices[4*index + 3].position += m_particles[index].velocity;

//             m_particles[index].lifetime--;
            
//         }
//     }

//     void draw(sf::RenderWindow& window) const
//     {
//         // std::cout << "draw" << std::endl;
//         window.draw(m_vertices);
//     }

// };
