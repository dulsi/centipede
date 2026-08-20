/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Spider class definition and implementation
*/

#include <random>
#include <vector>

#include "Spider.hpp"
#include "TextureManager.hpp"

Spider::Spider(gfx::FloatRect bounds) : AnimatingSprite(SpiderAnimationStates, SpiderAnimationOffset), m_rng{std::random_device{}()}
{
    setTexture(TextureManager::GetTexture("graphics/spider.png"));
    setCurrentAnimation(0);

    const auto& size = getLocalBounds().getSize();
    setOrigin(size.x / 2.f, size.y / 2.f);

    bounds.left += size.x / 2.f;
    bounds.top += size.y / 2.f;
    bounds.width -= size.x;
    bounds.height -= size.y;
    m_bounds = bounds;

    m_sound = SoundManager::GetManager().GetSound("sounds/bug_07.ogg");
    m_channel = -1;
    this->reset();
}

void Spider::reset()
{
    m_respawnTimer = 0;
    m_alive = false;
    if (m_channel != -1)
    {
        SoundManager::GetManager().Halt(m_channel);
    }
    m_channel = -1;
}

void Spider::spawn()
{
    setPosition(m_bounds.left, m_bounds.top);
    m_direction = Moving::UpRight;
    m_alive     = true;
    m_moveLeft = false;
    setCurrentAnimation(0);
    m_channel = SoundManager::GetManager().Play(m_sound, true);
}

void Spider::draw(SDL_Renderer* target) const
{
    if (m_alive)
    {
        gfx::Sprite::draw(target);
    }
}

void Spider::update(float deltaTime)
{
    if (!m_alive)
    {
        m_respawnTimer += deltaTime;
        if (m_respawnTimer >= m_respawnDuration)
        {
            this->spawn();
            m_respawnTimer = 0;
        }
        return;
    }

    float distance = Spider::Speed * deltaTime;
    switch (m_direction)
    {
    case Moving::Up:
        move(0.0, -distance);
        break;
    case Moving::Down:
        move(0.0, distance);
        break;
    case Moving::DownLeft:
        move(-distance, distance);
        break;
    case Moving::DownRight:
        move(distance, distance);
        break;
    case Moving::UpLeft:
        move(-distance, -distance);
        break;
    case Moving::UpRight:
        move(distance, -distance);
        break;
    }

    AnimatingSprite::update(deltaTime);
    m_moveTimer += deltaTime;
    if (m_moveTimer >= m_moveDuration)
    {
        std::vector<Moving> allowedDirections;
        if (getPosition().x >= m_bounds.left + m_bounds.width)
        {
            m_moveLeft = true;
        }
        else if (getPosition().x < m_bounds.left)
        {
            m_moveLeft = false;
        }

        if (m_moveLeft)
        {
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpLeft, Moving::DownLeft};
            if ((m_direction == Moving::UpLeft) || (m_direction == Moving::Up))
            {
                allowedDirections.push_back(Moving::Up);
                allowedDirections.push_back(Moving::UpLeft);
            }
            else
            {
                allowedDirections.push_back(Moving::Down);
                allowedDirections.push_back(Moving::DownLeft);
            }
        }
        else
        {
            allowedDirections = {Moving::Up, Moving::Down, Moving::UpRight, Moving::DownRight};
            if ((m_direction == Moving::UpRight) || (m_direction == Moving::Up))
            {
                allowedDirections.push_back(Moving::Up);
                allowedDirections.push_back(Moving::UpRight);
            }
            else
            {
                allowedDirections.push_back(Moving::Down);
                allowedDirections.push_back(Moving::DownRight);
            }
        }

        std::uniform_int_distribution<size_t> dist(0, allowedDirections.size() - 1);
        m_direction = allowedDirections[dist(m_rng)];

        m_moveTimer = 0;
    }

    if (getPosition().y < m_bounds.top)
    {
        switch (m_direction)
        {
        case Moving::UpLeft:
            m_direction = Moving::DownLeft;
            break;
        case Moving::Up:
            m_direction = Moving::Down;
            break;
        case Moving::UpRight:
            m_direction = Moving::DownRight;
            break;
        default:
            break;
        }
    }
    else if (getPosition().y >= m_bounds.top + m_bounds.height)
    {
        switch (m_direction)
        {
        case Moving::Down:
            m_direction = Moving::Up;
            break;
        case Moving::DownRight:
            m_direction = Moving::UpRight;
            break;
        case Moving::DownLeft:
            m_direction = Moving::UpLeft;
            break;
        default:
            break;
        }
    }
}

bool Spider::checkLaserCollision(gfx::FloatRect other)
{
    bool wasHit = m_alive && getGlobalBounds().intersects(other);
    if (wasHit)
    {
        m_alive = false;
        if (m_channel != -1)
        {
            SoundManager::GetManager().Halt(m_channel);
        }
        m_channel = -1;
    }
    return wasHit;
}

gfx::FloatRect Spider::getCollider() const
{
    return getGlobalBounds();
}

bool Spider::isDead() const
{
    return !m_alive;
}
