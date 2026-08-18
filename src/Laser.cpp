/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Laser object definition. These are instantiated once at runtime,
then activated/moved when needed.
*/

#include "Laser.hpp"

/**
 * Construct a new Laser object.
 * Set the size and color, and set origin to center.
 */
Laser::Laser() : gfx::RectangleShape{Laser::Size}
{
    setFillColor(Laser::Color);
    setOrigin(Laser::Size.x / 2.f, Laser::Size.y / 2.f);
    m_sound = SoundManager::GetManager().GetSound("sounds/laser.ogg");
}

/** Update sprite position based on elapsed seconds. */
void Laser::update(float deltaTime)
{
    // Don't do anything if not active
    if (!m_active)
    {
        return;
    }

    // distance traveled (up)
    const float distance = -Laser::Speed * deltaTime;

    move(0, distance);

    // deactivate when hitting the top of the screen
    if (getPosition().y < 0)
    {
        m_active = false;
    }
}

/** Only draw an active laser to the scene */
void Laser::draw(SDL_Renderer* target) const
{
    if (m_active)
    {
        gfx::RectangleShape::draw(target);
    }
}

void Laser::shoot(float x, float y, int player)
{
    m_active = true;

    setPosition(x, y);
    m_player = player;
    SoundManager::GetManager().Play(m_sound);
}

void Laser::shoot(gfx::Vector2f start, int player)
{
    shoot(start.x, start.y, player);
}

/** Return the boundary collider of this laser object. */
gfx::FloatRect Laser::getCollider() const
{
    return getGlobalBounds();
}

bool Laser::isActive() const
{
    return m_active;
}

void Laser::deactivate()
{
    m_active = false;
}

int Laser::getPlayer() const
{
    return m_player;
}
