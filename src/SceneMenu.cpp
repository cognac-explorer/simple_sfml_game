#include "SceneMenu.h"
#include "ScenePlay.h"
#include "Scene.h"
#include "GameEngine.h"
#include <iostream>
#include "raylib.h"


SceneMenu::SceneMenu(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void SceneMenu::init()
{
    m_title = "Menu";
    m_menuStrings = {"Level 1", "Level 2"};
    m_levelPaths = {"level.txt", "level2.txt"};

    registerAction(KEY_W,      "UP");
    registerAction(KEY_S,      "DOWN");
    registerAction(KEY_SPACE,  "OK");
    registerAction(KEY_ESCAPE, "QUIT");
}

void SceneMenu::update()
{
    sRender();
}

// void SceneMenu::onEnd()
// {
//     m_game -> quit();
// }

void SceneMenu::sDoAction(const Action& action)
{
    if (action.type() == "START" && action.name() == "UP")
    {
        m_selectedMenuIndex++;
        m_selectedMenuIndex = m_selectedMenuIndex % m_menuStrings.size();
    }
    else if (action.type() == "START" && action.name() == "DOWN")
    {
        m_selectedMenuIndex--;
        m_selectedMenuIndex = m_selectedMenuIndex % m_menuStrings.size();
    }
    else if (action.type() == "START" && action.name() == "OK")
    {
        m_game->changeScene("PLAY", std::make_shared<ScenePlay>(m_game, m_levelPaths[m_selectedMenuIndex]));
    }
    // else if (action.name() == "QUIT")  { onEnd(); }
}

void SceneMenu::sRender()
{
    BeginDrawing();
    ClearBackground(SKYBLUE);

    DrawText(m_title.c_str(), 100, 50, 45, BROWN);

    for (size_t i = 0; i < m_menuStrings.size(); ++i)
    {
        // Highlight the selected menu item
        if (i == m_selectedMenuIndex) 
        {
            DrawText(m_menuStrings[i].c_str(), 100, 250 + i * 70, 45, RED);
        }
        else 
        {
            DrawText(m_menuStrings[i].c_str(), 100, 250 + i * 70, 45, BROWN);
        }
    }

    EndDrawing();
}
