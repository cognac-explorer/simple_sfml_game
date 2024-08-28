#include "Scene.h"

#include "GameEngine.h"
#include "Action.h"
#include "EntityManager.h"


Scene::Scene()
    : m_game(nullptr), m_paused(false), m_hasEnded(false), m_currentFrame(0)
{
}

Scene::Scene(GameEngine * gameEngine)
    : m_game(gameEngine), m_paused(false), m_hasEnded(false), m_currentFrame(0)
{
}

void Scene::setPaused(bool paused)
{
    m_paused = paused;
}

void Scene::doAction(const Action & action)
{
    sDoAction(action);
}

void Scene::simulate(const size_t frames)
{
    for (size_t i = 0; i < frames; ++i)
    {
        if (!m_paused && !m_hasEnded)
        {
            update();
            ++m_currentFrame;
        }
    }
}

void Scene::registerAction(int inputKey, const std::string actionName)
{
    m_actionMap[inputKey] = actionName;
}

size_t Scene::width() const
{
    return m_game ? m_game->window().getSize().x : 0;
}

size_t Scene::height() const
{
    return m_game ? m_game->window().getSize().y : 0;
}

size_t Scene::currentFrame() const
{
    return m_currentFrame;
}

bool Scene::hasEnded() const
{
    return m_hasEnded;
}

const ActionMap & Scene::getActionMap() const
{
    return m_actionMap;
}
