/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
Scorpion class definition and implementation
*/

#include <random>

#include "Scorpion.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

Scorpion::Scorpion(gfx::FloatRect bounds) : m_rng{std::random_device{}()}
{
    setTexture(TextureManager::GetTexture("graphics/scorpion.png"));
    m_animation = 0;
    setTextureRect(Scorpion::ScorpionAnimationOffset[m_animation]);

    const auto& size = getLocalBounds().getSize();
    setOrigin(size.x / 2.f, size.y / 2.f);

    bounds.left += size.x / 2.f;
    bounds.top += size.y / 2.f;
    bounds.width -= size.x;
    bounds.height -= size.y;
    m_bounds = bounds;

    this->reset();
}

void Scorpion::reset()
{
    m_alive = false;
}

void Scorpion::spawn()
{
    std::uniform_int_distribution<size_t> dist(0, (size_t)(m_bounds.height / Game::GridSize));
    size_t y = dist(m_rng) * Game::GridSize;
    setPosition(m_bounds.left + m_bounds.width, m_bounds.top + (float)y);
    m_alive     = true;
    m_animation = 0;
    setTextureRect(Scorpion::ScorpionAnimationOffset[m_animation]);
}

void Scorpion::draw(SDL_Renderer* target) const
{
    if (m_alive)
    {
        gfx::Sprite::draw(target);
    }
}

void Scorpion::update(float deltaTime)
{
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
    move(-distance, 0.0);

    m_moveTimer += deltaTime;
    m_animationTimer += deltaTime;
    if (m_animationTimer >= m_animationDuration)
    {
        m_animation++;
        m_animation %= AnimationFrames;
        setTextureRect(Scorpion::ScorpionAnimationOffset[m_animation]);
        m_animationTimer = 0;
    }

    const auto& size = getLocalBounds().getSize();
    if (getPosition().x < m_bounds.left - size.x / 2.f)
    {
        m_alive = false;
    }
}

bool Scorpion::checkLaserCollision(gfx::FloatRect other)
{
    bool wasHit = m_alive && getGlobalBounds().intersects(other);
    if (wasHit)
    {
        m_alive = false;
    }
    return wasHit;
}

gfx::FloatRect Scorpion::getCollider() const
{
    return getGlobalBounds();
}

bool Scorpion::isDead() const
{
    return !m_alive;
}
