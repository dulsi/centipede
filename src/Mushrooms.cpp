/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
The MushroomManager and Shroom class definition.
*/

#include <algorithm>
#include <list>
#include <random>
#include <stdexcept>

#include "Mushrooms.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

Shroom::Shroom(int type, float x, float y) : m_type(type), m_poisoned(false), m_health(4)
{
    const auto& tex = TextureManager::GetTexture("graphics/mushroom.png");
    this->setTexture(tex);
    this->setTextureRect(NormalTexOffset[m_type][0]);

    const auto& size = this->getLocalBounds().getSize();
    this->setOrigin(size.x / 2.f, size.y / 2.f);
    this->setPosition(x, y);
}

Shroom::Shroom(int type, gfx::Vector2f location) : Shroom{type, location.x, location.y}
{
}

int Shroom::damage()
{
    if (m_health <= 0)
    {
        return 0;
    }

    m_health -= 1;

    auto& texOffset = m_poisoned ? PoisonTexOffset : NormalTexOffset;
    switch (m_health)
    {
    case 3:
        this->setTextureRect(texOffset[m_type][1]);
        break;
    case 2:
        this->setTextureRect(texOffset[m_type][2]);
        break;
    case 1:
        this->setTextureRect(texOffset[m_type][3]);
        break;
    case 0:
        break;
    default:
        throw std::runtime_error("Exhuastive switch failure: shroom.m_health");
    }

    return m_health;
}

gfx::Vector2f Shroom::getRightEdge() const
{
    const gfx::FloatRect& size   = this->getLocalBounds();
    const gfx::Vector2f&  center = this->getPosition();
    return gfx::Vector2f(center.x + size.width / 2.f, center.y);
}

gfx::Vector2f Shroom::getLeftEdge() const
{
    const gfx::FloatRect& size   = this->getLocalBounds();
    const gfx::Vector2f&  center = this->getPosition();
    return gfx::Vector2f(center.x - size.width / 2.f, center.y);
}

void Shroom::setType(int type)
{
    m_type = type;
    if (m_health > 0)
    {
        auto& texOffset = m_poisoned ? PoisonTexOffset : NormalTexOffset;
        this->setTextureRect(texOffset[m_type][4 - m_health]);
    }
}

bool Shroom::isPoisoned() const
{
    return m_poisoned;
}

void Shroom::poison()
{
    m_poisoned = true;
    if (m_health > 0)
    {
        auto& texOffset = m_poisoned ? PoisonTexOffset : NormalTexOffset;
        this->setTextureRect(texOffset[m_type][4 - m_health]);
    }
}

MushroomManager::MushroomManager(gfx::FloatRect bounds) : m_bounds(bounds), m_rng(std::random_device{}())
{
}

const std::list<Shroom>& MushroomManager::getShrooms() const
{
    return m_shrooms;
}

void MushroomManager::draw(SDL_Renderer* target) const
{
    for (const auto& shroom : m_shrooms)
    {
        shroom.draw(target);
    }
}

bool MushroomManager::checkSpiderCollision(gfx::FloatRect spider)
{
    auto isHit = [&](const Shroom& s) { return spider.intersects(s.getGlobalBounds()); };
    auto hit_it = std::find_if(m_shrooms.begin(), m_shrooms.end(), isHit);
    if (hit_it != m_shrooms.end())
    {
        m_shrooms.erase(hit_it);
        return true;
    }
    return false;
}

bool MushroomManager::checkScorpionCollision(gfx::FloatRect scorpion)
{
    auto isHit = [&](const Shroom& s) { return scorpion.intersects(s.getGlobalBounds()); };
    auto hit_it = std::find_if(m_shrooms.begin(), m_shrooms.end(), isHit);
    if (hit_it != m_shrooms.end())
    {
        hit_it->poison();
        return true;
    }
    return false;
}

bool MushroomManager::checkLaserCollision(gfx::FloatRect laser)
{
    auto isHit = [&](const Shroom& s) { return laser.intersects(s.getGlobalBounds()); };
    auto hit_it = std::find_if(m_shrooms.begin(), m_shrooms.end(), isHit);
    if (hit_it != m_shrooms.end())
    {
        int remaining_health = hit_it->damage();
        if (remaining_health <= 0)
        {
            m_shrooms.erase(hit_it);
        }
        return true;
    }
    return false;
}

void MushroomManager::addMushroom(gfx::Vector2f location)
{
    location.x = static_cast<float>(static_cast<int>(location.x - (Game::GridSize / 2.f)) / Game::GridSize * Game::GridSize) + Game::GridSize / 2.f;
    location.y = static_cast<float>(static_cast<int>(location.y - (Game::GridSize / 2.f)) / Game::GridSize * Game::GridSize) + Game::GridSize / 2.f;
    auto iter = std::find_if(m_shrooms.begin(), m_shrooms.end(), [location](const Shroom& shroom){
        auto position = shroom.getPosition();
        return position.x == location.x && position.y == location.y;
    });
    if (iter == m_shrooms.end())
    {
        m_shrooms.emplace_back(m_type, location);
    }
}

void MushroomManager::nextLevel()
{
    m_type++;
    m_type %= 3;
    m_levelChangeTimer = 0;
}

bool MushroomManager::update(float deltaTime)
{
    m_levelChangeTimer += deltaTime;
    if (m_levelChangeTimer >= m_levelChangeDuration)
    {
        for (auto& shroom : m_shrooms)
        {
            shroom.setType(m_type);
        }
        return true;
    }
    return false;
}

void MushroomManager::reset()
{
    m_type = 0;
    m_shrooms.clear();

    auto                               x_range = static_cast<int>(m_bounds.width / Game::GridSize) - 1;
    auto                               y_range = static_cast<int>(m_bounds.height / Game::GridSize) - 1;
    std::uniform_int_distribution<int> random_x(0, x_range);
    std::uniform_int_distribution<int> random_y(0, y_range);

    for (size_t i = 0; i < 35; ++i)
    {
        const float gridx = static_cast<float>(Game::GridSize * random_x(m_rng));
        const float gridy = static_cast<float>(Game::GridSize * random_y(m_rng));
        const float xPos  = m_bounds.left + gridx + Game::GridSize / 2.f;
        const float yPos  = m_bounds.top + gridy + Game::GridSize / 2.f;
        m_shrooms.emplace_back(m_type, xPos, yPos);
    }
}
