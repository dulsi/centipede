/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
Ant class definition and implementation
*/

#include <random>

#include "SFML/Graphics.hpp"

#include "Ant.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

/** Construction and set up the inherited Sprite properties */
Ant::Ant(sf::FloatRect bounds, MushroomManager& shroomMan) : m_rng{std::random_device{}()}, m_shroomMan{shroomMan}
{
    m_sprite.setTexture(TextureManager::GetTexture("graphics/ant.png"));
    m_animation = 0;
    m_sprite.setTextureRect(Ant::AntAnimationOffset[m_animation]);
    m_health = 2;

    const auto& size = m_sprite.getLocalBounds().getSize();
    m_sprite.setOrigin(size.x / 2.f, size.y / 2.f);

    bounds.left += size.x / 2.f;
    bounds.top += size.y / 2.f;
    bounds.width -= size.x;
    bounds.height -= size.y;
    m_bounds = bounds;

    this->reset();
}

void Ant::reset()
{
    m_respawnNow = false;
    m_alive = false;
}

void Ant::spawn()
{
    std::uniform_int_distribution<size_t> dist(0, (size_t)(m_bounds.width / Game::GridSize));
    size_t x = dist(m_rng) * Game::GridSize;
    //const auto& size = m_sprite.getLocalBounds().getSize();
    m_sprite.setPosition(m_bounds.left + (float)x, m_bounds.top);
    m_alive     = true;
    m_animation = 0;
    m_health = 2;
    m_sprite.setTextureRect(Ant::AntAnimationOffset[m_animation]);
}

void Ant::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_alive)
    {
        target.draw(m_sprite, states);
    }
}

void Ant::update(float deltaTime)
{
    // if currently inactive, increment timer and don't move around
    if (!m_alive)
    {
        if ((m_shroomMan.getShrooms().size() < 25) || (m_respawnNow))
        {
            this->spawn();
            m_respawnNow = false;
        }
        return;
    }

    float distance = Ant::Speed * deltaTime;
    m_sprite.move(0.0, distance);

    m_moveTimer += deltaTime;
    m_animationTimer += deltaTime;
    if (m_animationTimer >= m_animationDuration)
    {
        m_animation++;
        m_animation %= AnimationFrames;
        m_sprite.setTextureRect(Ant::AntAnimationOffset[m_animation]);
        m_animationTimer = 0;
    }

    const auto& size = m_sprite.getLocalBounds().getSize();
    if (m_sprite.getPosition().y > m_bounds.top + m_bounds.height + size.x / 2.f)
    {
        m_alive = false;
    }
    else if ((int)(m_sprite.getPosition().y) % Game::GridSize < 5)
    {
        std::uniform_int_distribution<int> dist(0, 100);
        int roll = dist(m_rng);
        if (m_mushroomChance >= roll)
        {
            auto where = m_sprite.getPosition();
            where.y = (float)(((int)where.y / Game::GridSize) * Game::GridSize) + Game::GridSize / 2.f;
            m_shroomMan.addMushroom(where);
        }
    }
}

/**  */
bool Ant::checkLaserCollision(sf::FloatRect other)
{
    // only living ants can be hit
    bool wasHit = m_alive && m_sprite.getGlobalBounds().intersects(other);
    if (wasHit)
    {
        m_health--;
        if (m_health == 0)
        {
            m_alive = false;
            m_respawnNow = true;
        }
    }
    return wasHit;
}

sf::FloatRect Ant::getCollider() const
{
    return m_sprite.getGlobalBounds();
}

bool Ant::isDead() const
{
    return !m_alive;
}
