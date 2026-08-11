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
Laser::Laser() : m_shape{Laser::Size}
{
    m_shape.setFillColor(Laser::Color);
    m_shape.setOrigin(Laser::Size.x / 2.f, Laser::Size.y / 2.f);
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

    m_shape.move(0, distance);

    // deactivate when hitting the top of the screen
    if (m_shape.getPosition().y < 0)
    {
        m_active = false;
    }
}

/** Only draw an active laser to the scene */
void Laser::draw(gfx::RenderTarget& target) const
{
    if (m_active)
    {
        target.draw(m_shape);
    }
}

void Laser::shoot(float x, float y, int player)
{
    m_active = true;

    m_shape.setPosition(x, y);
    m_player = player;
}

void Laser::shoot(gfx::Vector2f start, int player)
{
    shoot(start.x, start.y, player);
}

/** Return the boundary collider of this laser object. */
gfx::FloatRect Laser::getCollider() const
{
    return m_shape.getGlobalBounds();
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
