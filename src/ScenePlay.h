#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <Physics.h>
#include "ParticleSystem.cpp"


class ScenePlay : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAXSPEED, JUMPSPEED, GRAVITY;
        std::string WEAPON;
    };

protected:

    Camera2D m_camera;
    std::shared_ptr<Entity> m_player;
    std::string             m_levelPath;
    PlayerConfig            m_playerConfig;
    bool                    m_drawTextures = true;
    bool                    m_drawCollision = false;
    bool                    m_drawGrid = false;
    const Vec2              m_gridSize = {64, 64};
    std::string             m_gridText;
    Physics                 m_physics;
    ParticleSystem          m_particles;

    void init(const std::string& levelPath);

public:
    ScenePlay(GameEngine* gameEngine, const std::string& levelPath);
    void loadLevel(const std::string& fileName);
    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);
    void sMovement();
    void sRender();
    void sAnimation();
    void sLifespan();
    void sCollision();
    void sParallax();
    // void sParticles();
    void sDoAction(const Action& action);
    void onEnd();
    void update();
};
