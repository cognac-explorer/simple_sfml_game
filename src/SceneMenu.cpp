#include "SceneMenu.h"
#include "ScenePlay.h"
#include "Scene.h"
#include "GameEngine.h"
#include <SFML/Graphics.hpp>
#include <iostream>


SceneMenu::SceneMenu(GameEngine * gameEngine)
    : Scene(gameEngine)
{
    init();
}

void SceneMenu::init()
{
    m_title = "Menu";
    m_menuStrings = {"Level 1", "Level 2"};
    m_levelPaths = {"../bin/level.txt", "../bin/level2.txt"};

    registerAction(sf::Keyboard::W,      "UP");
    registerAction(sf::Keyboard::S,      "DOWN");
    registerAction(sf::Keyboard::Space,  "OK");
    registerAction(sf::Keyboard::Escape, "QUIT");
}

void SceneMenu::update()
{
    sRender();
}

void SceneMenu::onEnd()
{
    m_game -> quit();
}

void SceneMenu::sDoAction(const Action & action)
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
    else if (action.name() == "QUIT")  { onEnd(); }
}

void SceneMenu::sRender()
{
    m_game->window().clear(sf::Color(50, 50, 150));
    // reset game view
    sf::View view = m_game->window().getView();
    view.setCenter(m_game->window().getSize().x / 2, m_game->window().getSize().y / 2);
    m_game->window().setView(view);

    m_menuText.setString(m_title);
    m_menuText.setFont(m_game->getAssets().getFont("Main"));
    m_menuText.setPosition(100, 50);
    m_menuText.setCharacterSize(50);
    m_menuText.setFillColor(sf::Color::Green);
    m_menuText.setLetterSpacing(1.5);
    m_game->window().draw(m_menuText);

    for (size_t i = 0; i < m_menuStrings.size(); ++i)
    {
        sf::Text levelName;
        levelName.setFont(m_game->getAssets().getFont("Main"));
        levelName.setString(m_menuStrings[i]);
        levelName.setPosition(100, 250 + i * 70);
        
        // Highlight the selected menu item
        if (i == m_selectedMenuIndex) 
        {
            levelName.setFillColor(sf::Color::Red);
        }
        else 
        {
            levelName.setFillColor(sf::Color::White);
        }

        m_game->window().draw(levelName);
    }
    
    sf::Text bottomText;
    bottomText.setFont(m_game->getAssets().getFont("Main"));
    bottomText.setString("Use W key for up, S for down, space for enter and ESC for exit");
    bottomText.setPosition(100, 700);
    bottomText.setCharacterSize(20);
    m_game->window().draw(bottomText);

    m_game->window().display();
}
