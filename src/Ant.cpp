/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
Ant class definition and implementation
*/

#include <random>

#include "Ant.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

Ant::Ant(gfx::FloatRect bounds, MushroomManager& shroomMan) : m_rng{std::random_device{}()}, m_shroomMan{shroomMan}
{
    setTexture(TextureManager::GetTexture("graphics/ant.png"));
    m_animation = 0;
    setTextureRect(Ant::AntAnimationOffset[m_animation]);
    m_health = 2;

    const auto& size = getLocalBounds().getSize();
    setOrigin(size.x / 2.f, size.y / 2.f);

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
    setPosition(m_bounds.left + (float)x, m_bounds.top);
    m_alive     = true;
    m_animation = 0;
    m_health = 2;
    setTextureRect(Ant::AntAnimationOffset[m_animation]);
}

void Ant::draw(gfx::RenderTarget& target) const
{
    if (m_alive)
    {
        gfx::Sprite::draw(target);
    }
}

void Ant::update(float deltaTime)
{
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
    move(0.0, distance);

    m_moveTimer += deltaTime;
    m_animationTimer += deltaTime;
    if (m_animationTimer >= m_animationDuration)
    {
        m_animation++;
        m_animation %= AnimationFrames;
        setTextureRect(Ant::AntAnimationOffset[m_animation]);
        m_animationTimer = 0;
    }

    const auto& size = getLocalBounds().getSize();
    if (getPosition().y > m_bounds.top + m_bounds.height + size.x / 2.f)
    {
        m_alive = false;
    }
    else if ((int)(getPosition().y) % Game::GridSize < 5)
    {
        std::uniform_int_distribution<int> dist(0, 100);
        int roll = dist(m_rng);
        if (m_mushroomChance >= roll)
        {
            auto where = getPosition();
            where.y = (float)(((int)where.y / Game::GridSize) * Game::GridSize) + Game::GridSize / 2.f;
            m_shroomMan.addMushroom(where);
        }
    }
}

bool Ant::checkLaserCollision(gfx::FloatRect other)
{
    bool wasHit = m_alive && getGlobalBounds().intersects(other);
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

gfx::FloatRect Ant::getCollider() const
{
    return getGlobalBounds();
}

bool Ant::isDead() const
{
    return !m_alive;
}
