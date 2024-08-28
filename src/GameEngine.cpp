#include "GameEngine.h"
#include "Assets.h"
#include "ScenePlay.h"
#include "SceneMenu.h"

#include <iostream>

GameEngine::GameEngine(const std::string & path)
{
    init(path);
}

void GameEngine::init(const std::string & path)
{
    m_assets.loadFromFile(path);
    m_window.create(sf::VideoMode(1280, 768), "Test");
    m_window.setFramerateLimit(60);
    // m_sceneMap["MENU"] = std::make_shared<SceneMenu>(this);
    // m_sceneMap["PLAY"] = std::make_shared<ScenePlay>(this, "../bin/level.txt");
    // m_currentScene = "MENU";
    changeScene("MENU", std::make_shared<SceneMenu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
    return m_runnig & m_window.isOpen();
}

sf::RenderWindow & GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while(isRunning())
    {
        sUserInput();
        update();
    }
}

void GameEngine::update()
{
    currentScene() -> update();
}

void GameEngine::quit()
{
    m_runnig = false;
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START": "END";
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
}

void GameEngine::changeScene(const std::string & sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = scene;

}

const Assets & GameEngine::getAssets() const
{
    return m_assets;
}
