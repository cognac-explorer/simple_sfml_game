#include "EntityManager.h"
#include <iostream>
#include <algorithm>

EntityManager::EntityManager()
{
    
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}

void EntityManager::update()
{
    for (auto e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entitiesMap[e->m_tag].push_back(e);
    }
    m_entitiesToAdd.clear();

    removeDeadEntities(m_entities);
    for (auto& [tag, entityVec] : m_entitiesMap)
    {
        removeDeadEntities(entityVec);
    }
}

const EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
    return m_entitiesMap[tag];
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                            [](const std::shared_ptr<Entity>& e) 
                            {
                                return e->isActive() == false;
                            }),
                            vec.end()
    );
}

int EntityManager::len() const
{
    return m_entities.size();
}
