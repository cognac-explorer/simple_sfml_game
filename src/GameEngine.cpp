#include "GameEngine.h"
#include "ScenePlay.h"
#include "SceneMenu.h"

#include <iostream>
#include "raylib.h"

GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

void GameEngine::init(const std::string& path)
{
    InitWindow(1280, 768, "SNT");
    SetTargetFPS(60);
    m_assets.loadFromFile(path);
    m_sceneMap["MENU"] = std::make_shared<SceneMenu>(this);
    m_sceneMap["PLAY"] = std::make_shared<ScenePlay>(this, "level.txt");
    changeScene("MENU", std::make_shared<SceneMenu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
    return m_runnig;
}

void GameEngine::run()
{
    while(!WindowShouldClose() && isRunning())
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
    CloseWindow();
}

void GameEngine::sUserInput()
{
    for (auto& [keyCode, actionName] : currentScene()->getActionMap())
    {
        if (IsKeyPressed(keyCode))  // Key just pressed
        {
            currentScene()->doAction(Action(actionName, "START"));
            std::cout << "keyCode = " << keyCode << " actionName = " << actionName << std::endl;
        }
        if (IsKeyReleased(keyCode)) // Key just released
        {
            currentScene()->doAction(Action(actionName, "END"));
        }
    }
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = scene;

}

const Assets& GameEngine::getAssets() const
{
    return m_assets;
}
