#include "ScenePlay.h"
#include "SceneMenu.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <fstream>
#include <sstream>
#include <iostream>

ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}

void ScenePlay::init(const std::string& levelPath)
{
    registerAction(KEY_P,      "PAUSE");
    registerAction(KEY_ESCAPE, "QUIT");
    registerAction(KEY_T,      "TOGGLE_TEXTURE");
    registerAction(KEY_C,      "TOGGLE_COLLISION");
    registerAction(KEY_G,      "TOGGLE_GRID");
    registerAction(KEY_A,      "LEFT");
    registerAction(KEY_D,      "RIGHT");
    registerAction(KEY_W,      "JUMP");
    registerAction(KEY_SPACE,  "SHOOT");

    loadLevel(levelPath);

    m_camera.target = {0.0f, 0.0f};      // What the camera looks at
    m_camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};  // Camera center
    m_camera.rotation = 0.0f;
    m_camera.zoom = 1.0f;

    // m_particles.init(m_game->window().getSize());
}

void ScenePlay::spawnPlayer()
{
    // read player config from file
    std::ifstream inputFile("player.txt");
    if (!inputFile)
    {
        std::cerr << "Error opening file player.txt" << std::endl;
    }

    std::string line;
    if (std::getline(inputFile, line))  // skip first line
    {
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            int X, Y, CX, CY, speed, speedJump, speedMax;
            float gravity;
            std::string bulletAnimation;
            iss >> X >> Y >> CX >> CY >> speed >> speedJump >> speedMax >> gravity;
            m_playerConfig.X = X;
            m_playerConfig.Y = Y;
            m_playerConfig.CX = CX;
            m_playerConfig.CY = CY;
            m_playerConfig.SPEED = speed;
            m_playerConfig.JUMPSPEED = speedJump;
            m_playerConfig.MAXSPEED = speedMax;
            m_playerConfig.GRAVITY = gravity;
            m_playerConfig.WEAPON = bulletAnimation;
        }
    }

    m_player = m_entityManager.addEntity("player");
    Animation animationPlayer = m_game->getAssets().getAnimation("Stand");
    m_player->addComponent<CAnimation>(animationPlayer, true);
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X , m_playerConfig.Y));   
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX , m_playerConfig.CY));
    m_player->addComponent<CState>("Stand");
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->addComponent<CInput>();

}

Vec2 ScenePlay::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    Vec2 size = entity->getComponent<CAnimation>().animation.getSize();
    return Vec2(gridX*m_gridSize.x + size.x / 2, GetScreenHeight() - gridY*m_gridSize.y - size.y / 2);
}

void ScenePlay::loadLevel(const std::string& fileName)
{
    std::ifstream inputFile(fileName);
    if (!inputFile)
    {
        std::cerr << "Error opening file " << fileName << std::endl;
    }

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string entityType;
        iss >> entityType;
        if (entityType == "Tile")
        {
            std::string tileName; 
            int gridX, gridY;
            iss >> tileName >> gridX >> gridY;
            auto brick = m_entityManager.addEntity("tile");
            Animation animationTile = m_game->getAssets().getAnimation(tileName);
            brick->addComponent<CAnimation>(animationTile, false);
            brick->addComponent<CBoundingBox>(animationTile.getSize());
            brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));
        }
        if (entityType == "Dec")
        {
            std::string tileName; 
            int gridX, gridY;
            iss >> tileName >> gridX >> gridY;
            auto brick = m_entityManager.addEntity("dec");
            Animation animationTile = m_game->getAssets().getAnimation(tileName);
            brick->addComponent<CAnimation>(animationTile, false);
            brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));
            if (tileName == "Cloud")
            {
                brick->addComponent<CParallax>(0.15);
            }
        }
    }

    inputFile.close();
    spawnPlayer();
}

void ScenePlay::spawnBullet(std::shared_ptr<Entity> entity)
{
    auto arrow = m_entityManager.addEntity("arrow");
    Animation animationArrow = m_game->getAssets().getAnimation("Arrow");
    arrow->addComponent<CAnimation>(animationArrow, false);
    arrow->addComponent<CBoundingBox>(animationArrow.getSize());
    arrow->addComponent<CTransform>(entity->getComponent<CTransform>().pos);
    arrow->getComponent<CTransform>().velocity = Vec2(12, 0);
    arrow->addComponent<CLifespan>(45, m_currentFrame);
}

void ScenePlay::sMovement()
{
    Vec2& playerVelocity = m_player->getComponent<CTransform>().velocity;
    if (m_player->getComponent<CInput>().right)
    {
        m_player->getComponent<CTransform>().facingLeft = false;
        m_player->addComponent<CState>("Run");
        playerVelocity.x = m_playerConfig.SPEED;
    }
    else if (m_player->getComponent<CInput>().left)
    {
        m_player->getComponent<CTransform>().facingLeft = true;
        
        m_player->addComponent<CState>("Run");
        playerVelocity.x = -m_playerConfig.SPEED;
    }
    else
    {
        playerVelocity.x = 0;
        m_player->addComponent<CState>("Stand");
    }

    if (m_player->getComponent<CInput>().up && m_player->getComponent<CInput>().canJump)
    {
        playerVelocity.y -= m_playerConfig.JUMPSPEED;
    }
    if (m_player->getComponent<CInput>().shoot && m_player->getComponent<CInput>().canShoot)
    {
        spawnBullet(m_player);

        m_player->addComponent<CState>("Shoot");
        m_player->getComponent<CInput>().canShoot = false;
    }

    // don't allow player velocity to exceed max velocity
    if (playerVelocity.x > m_playerConfig.MAXSPEED)
    {
        playerVelocity.x = m_playerConfig.MAXSPEED;
    }
    else if (playerVelocity.x < -m_playerConfig.MAXSPEED)
    {
        playerVelocity.x = -m_playerConfig.MAXSPEED;
    }
    if (playerVelocity.y < -m_playerConfig.MAXSPEED)
    {
        playerVelocity.y = -m_playerConfig.MAXSPEED;
    }
    else if (playerVelocity.y < -m_playerConfig.MAXSPEED)
    {
        playerVelocity.y = -m_playerConfig.MAXSPEED;
    }

    for (auto e : m_entityManager.getEntities())
    {
        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
        Vec2& velocity = e->getComponent<CTransform>().velocity;
        if (e->hasComponent<CGravity>())
        {
            velocity.y += e->getComponent<CGravity>().gravity;
        }
        e->getComponent<CTransform>().pos += velocity;
    }
    // if player fall below
    if (m_player->getComponent<CTransform>().pos.y > GetScreenHeight())
    {
        m_player->destroy();
        // onEnd();
    }
}

void ScenePlay::sRender()
{
    // if (!m_paused)
    // {
    //     m_game->window().clear(sf::Color(100, 100, 255));
    // }
    // else
    // {
    //     m_game->window().clear(sf::Color(50, 50, 150));
    // }

    // align view with player x position
    auto& pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(GetScreenWidth() / 2.0f, pPos.x);
    m_camera.target = {windowCenterX, GetScreenHeight() / 2.0f};

    BeginDrawing();
    ClearBackground(BLUE);
    BeginMode2D(m_camera);

    for (auto e : m_entityManager.getEntities())
    {
        auto& transform = e->getComponent<CTransform>();

        if (m_drawTextures && e->hasComponent<CAnimation>())
        {
            auto& animation = e->getComponent<CAnimation>().animation;
            animation.draw(transform.pos, transform.scale, transform.angle, transform.facingLeft);
        }

        // if (m_drawCollision && e->hasComponent<CBoundingBox>())
        // {
        //     Vec2& size = e->getComponent<CBoundingBox>().size;
        //     sf::RectangleShape rect;
        //     rect.setOrigin(size.x / 2.0f, size.y / 2.0f);
        //     rect.setPosition(transform.pos.x, transform.pos.y);
        //     rect.setSize(sf::Vector2(size.x, size.y));
        //     rect.setFillColor(sf::Color::Transparent);
        //     rect.setOutlineColor(sf::Color::Magenta);
        //     rect.setOutlineThickness(3.0);
        //     m_game->window().draw(rect);
        // }
    }

    // if (m_drawGrid)
    // {
    //     sf::VertexArray verticalLines(sf::Lines);
    //     sf::VertexArray horizontalLines(sf::Lines);

    //     for (int x = 0; x < view.getCenter().x + m_game->window().getSize().x; x += m_gridSize.x)
    //     {
    //         verticalLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color::Black));
    //         verticalLines.append(sf::Vertex(sf::Vector2f(x, m_game->window().getSize().y), sf::Color::Black));
    //     }
    //     for (int y = 0; y < m_game->window().getSize().y; y += m_gridSize.y)
    //     {
    //         horizontalLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color::Black));
    //         horizontalLines.append(sf::Vertex(sf::Vector2f(view.getCenter().x + m_game->window().getSize().x, y), sf::Color::Black));
    //     }
    //     m_game->window().draw(verticalLines);
    //     m_game->window().draw(horizontalLines);

    //     for (int x = 0; x < view.getCenter().x + m_game->window().getSize().x; x += m_gridSize.x) {
    //         for (int y = 0; y < m_game->window().getSize().y; y += m_gridSize.y) 
    //         {
    //             std::stringstream ss;
    //             ss << "(" << x / m_gridSize.x << ", " << (m_game->window().getSize().y - y) / m_gridSize.y - 1 << ")";
    //             m_gridText.setString(ss.str());
    //             m_gridText.setCharacterSize(10);
    //             m_gridText.setFillColor(sf::Color::Black);
    //             m_gridText.setPosition(x + 5, y + 5); // Offset the text slightly from the grid intersection
    //             m_game->window().draw(m_gridText);
    //         }
    //     }
    // }
    
    // m_particles.update();
    // m_particles.draw(m_game->window());

    // m_game->window().display();

    EndMode2D();  // Stop using camera
    EndDrawing();
}

void ScenePlay::sAnimation()
{
    std::string playerState = m_player->getComponent<CState>().state;
    Animation playerAnim = m_player->getComponent<CAnimation>().animation;
    
    if (playerState == "Stand" && playerAnim.getName() != "Stand" && playerAnim.getName() != "Shoot")
    {
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Stand"), true);
    }
    else if (playerState == "Run" && playerAnim.getName() != "Run" && playerAnim.getName() != "Shoot")
    {
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Run"), true);
    }
    else if (playerState == "Jump" && playerAnim.getName() != "Jump" && playerAnim.getName() != "Shoot")
    {
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Jump"), false);
    }
    else if (playerState == "Shoot" && playerAnim.getName() != "Shoot")
    {
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Shoot"), false);
    }

    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CAnimation>())
        {
            Animation& entityAnimation = e->getComponent<CAnimation>().animation;
            entityAnimation.update();
            if (entityAnimation.hasEnded() && !e->getComponent<CAnimation>().repeat)
            {
                if (e->tag() == "tile")
                {
                    e->destroy();
                }
                else if (e->tag() == "player")
                {
                    e->addComponent<CAnimation>(m_game->getAssets().getAnimation("Stand"), true);
                }
            }
            else if (entityAnimation.hasEnded() && e->getComponent<CAnimation>().repeat)
            {
                e->addComponent<CAnimation>(m_game->getAssets().getAnimation(entityAnimation.getName()), true);
            }
        }
    }
}

void ScenePlay::sLifespan()
{
    for (auto e : m_entityManager.getEntities("arrow"))
    {
        auto& lifespanComp = e->getComponent<CLifespan>();
        if (m_currentFrame - lifespanComp.frameCreated >= lifespanComp.lifespan)
        {
            e->destroy();
        }
    }
}

void ScenePlay::sParallax()
{
    Vec2& playerVelocity = m_player->getComponent<CTransform>().velocity;
    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CParallax>())
        {
            Vec2& velocity = e->getComponent<CTransform>().velocity;
            velocity.x = playerVelocity.x * e->getComponent<CParallax>().value;
        }
    }
}

void ScenePlay::sCollision()
{
    bool is_in_air = true;

    for (auto tile : m_entityManager.getEntities("tile"))
    {
        Vec2 overlap = m_physics.GetOverlap(m_player, tile);
        Vec2 prevOverlap = m_physics.GetPreviousOverlap(m_player, tile);
        
        if (overlap.x > 0 && overlap.y > 0)
        {
            if (prevOverlap.x > 0 && m_player->getComponent<CTransform>().pos.y < tile->getComponent<CTransform>().pos.y)
            {
                m_player->getComponent<CTransform>().pos.y -= overlap.y;
                m_player->getComponent<CTransform>().velocity.y = 0;
                m_player->getComponent<CInput>().canJump = true;
                is_in_air = false;
            }
            if (prevOverlap.x > 0 && m_player->getComponent<CTransform>().pos.y > tile->getComponent<CTransform>().pos.y)
            {
                m_player->getComponent<CTransform>().pos.y += overlap.y;
                m_player->getComponent<CTransform>().velocity.y = 0;
            }
            if (prevOverlap.y > 0 && m_player->getComponent<CTransform>().pos.x < tile->getComponent<CTransform>().pos.x)
            {
                m_player->getComponent<CTransform>().pos.x -= overlap.x;
                m_player->getComponent<CTransform>().velocity.x = 0;
            }
            if (prevOverlap.y > 0 && m_player->getComponent<CTransform>().pos.x > tile->getComponent<CTransform>().pos.x)
            {
                m_player->getComponent<CTransform>().pos.x += overlap.x;
                m_player->getComponent<CTransform>().velocity.x = 0;
            }
            if (tile->getComponent<CAnimation>().animation.getName() == "Finish")
            {
                // onEnd();
            }
        }
    }

    for (auto tile : m_entityManager.getEntities("tile"))
    {
        for (auto arrow : m_entityManager.getEntities("arrow"))
        {
            Vec2 overlap = m_physics.GetOverlap(arrow, tile);
            if (overlap.x > 0 && overlap.y > 0)
            {
                arrow->destroy();
            }
        }
    }
    if (is_in_air)
    {
        m_player->addComponent<CState>("Jump");
        m_player->getComponent<CInput>().canJump = false;
    }
}

void ScenePlay::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
             if (action.name() == "TOGGLE_TEXTURE")    { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION")  { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID")       { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE" )            { setPaused(!m_paused); }
        // else if (action.name() == "QUIT")              { onEnd(); }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "JUMP")
        {
            m_player->getComponent<CInput>().up = true;
        }
        else if (action.name() == "SHOOT")
        {
            m_player->getComponent<CInput>().shoot = true;
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
        }
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "JUMP")
        {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "SHOOT")
        {
            m_player->getComponent<CInput>().shoot = false;
            m_player->getComponent<CInput>().canShoot = true;
        }
    }
}

// void ScenePlay::sParticles()
// {
//     m_particles.update();
//     m_particles.draw(m_game->window());
//     m_game->window().display();
// }

// void ScenePlay::onEnd()
// {
//     int current = 0;
//     int max = 60;  // draw for 60 frames
    
//     m_game->window().clear(sf::Color(50, 50, 150));
//     m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
//     sf::Text t;
//     t.setString("You lose");
//     t.setCharacterSize(50);
//     t.setFillColor(sf::Color::Black);
//     t.setFont(m_game->getAssets().getFont("Main"));
//     t.setPosition(m_game->window().getView().getCenter().x, GetScreenHeight() / 2);
//     m_game->window().draw(t);
//     while (current < max) 
//     {
//         m_game->window().display();
//         current++;
//     }
    
// }

void ScenePlay::update()
{
    m_entityManager.update();

    // TODO paused

    sMovement();
    sLifespan();
    sCollision();
    sParallax();
    sAnimation();
    sRender();
    // sParticles();
    m_currentFrame++;
}
