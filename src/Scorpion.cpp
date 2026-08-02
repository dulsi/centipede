/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
Scorpion class definition and implementation
*/

#include <random>

#include "SFML/Graphics.hpp"

#include "Scorpion.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

/** Construction and set up the inherited Sprite properties */
Scorpion::Scorpion(sf::FloatRect bounds) : m_rng{std::random_device{}()}
{
    m_sprite.setTexture(TextureManager::GetTexture("graphics/scorpion.png"));
    m_animation = 0;
    m_sprite.setTextureRect(Scorpion::ScorpionAnimationOffset[m_animation]);

    const auto& size = m_sprite.getLocalBounds().getSize();
    m_sprite.setOrigin(size.x / 2.f, size.y / 2.f);

    bounds.left += size.x / 2.f;
    bounds.top += size.y / 2.f;
    bounds.width -= size.x;
    bounds.height -= size.y;
    m_bounds = bounds;

    this->reset();
}

void Scorpion::reset()
{
    m_respawnTimer = 0;
    m_alive = false;
}

void Scorpion::spawn()
{
    std::uniform_int_distribution<size_t> dist(0, (size_t)(m_bounds.height / Game::GridSize));
    size_t y = dist(m_rng) * Game::GridSize;
    m_sprite.setPosition(m_bounds.left + m_bounds.width, m_bounds.top + (float)y);
    m_alive     = true;
    m_animation = 0;
    m_sprite.setTextureRect(Scorpion::ScorpionAnimationOffset[m_animation]);
}

void Scorpion::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_alive)
    {
        target.draw(m_sprite, states);
    }
}

void Scorpion::update(float deltaTime)
{
    // if currently inactive, increment timer and don't move around
    if (!m_alive)
    {
        std::uniform_int_distribution<size_t> dist(0, 1000);
        if (dist(m_rng) < m_spawnChance)
        {
            this->spawn();
        }
        return;
    }

    float distance = Scorpion::Speed * deltaTime;
    m_sprite.move(-distance, 0.0);

    m_moveTimer += deltaTime;
    m_animationTimer += deltaTime;
    if (m_animationTimer >= m_animationDuration)
    {
        m_animation++;
        m_animation %= AnimationFrames;
        m_sprite.setTextureRect(Scorpion::ScorpionAnimationOffset[m_animation]);
        m_animationTimer = 0;
    }

    const auto& size = m_sprite.getLocalBounds().getSize();
    if (m_sprite.getPosition().x < m_bounds.left - size.x / 2.f)
    {
        m_alive = false;
    }
}

/**  */
bool Scorpion::checkLaserCollision(sf::FloatRect other)
{
    // only living scorpions can be hit
    bool wasHit = m_alive && m_sprite.getGlobalBounds().intersects(other);
    if (wasHit)
    {
        m_alive = false;
    }
    return wasHit;
}

sf::FloatRect Scorpion::getCollider() const
{
    return m_sprite.getGlobalBounds();
}

bool Scorpion::isDead() const
{
    return !m_alive;
}
