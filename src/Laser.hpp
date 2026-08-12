/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Laser object declaration. These are activated when the <SPACE> key is pressed.
The actual Atari Centipede game only allowed one laser sprite on screen at a time.
I might go that route, it would simplify the fire-rate timing.
*/

#pragma once

#include "gfx/RectangleShape.hpp"
#include "gfx/RenderWindow.hpp"
#include "gfx/Types.hpp"

/**
 * Laser objects that can be recycled throughout the scene.
 * Instances that are not `Laser::active` should not be drawn or updated.
 */
class Laser : public gfx::RectangleShape
{
  public:
    /** Fire-rate of all Laser instances (shots/second) */
    static constexpr double FireRate = 6.5;

    /** Construct a new Laser. */
    Laser();

    /**
     * Move the beam straight up (negative y).
     *
     * Automatically deactivates when reaching the top.
     * @param deltaTime Elapsed time in seconds
     */
    void update(float deltaTime);

    void draw(SDL_Renderer* target) const;

    /**
     * Make this Laser active, and set it's position to (x,y)
     * @param x pos of start
     * @param y pos of start
     * @param player person shooting
     */
    void shoot(float xStart, float yStart, int player);

    /**
     * Make this Laser active, and set it's position to `start`
     * @param start vector position to start from
     * @param player person shooting
     */
    void shoot(gfx::Vector2f start, int player);

    /**
     * Get the boundary collider for this laser.
     * For use with collision detection
     *
     * @return gfx::FloatRect global bounds rectangle of the laser
     */
    gfx::FloatRect getCollider() const;

    /** Change the state of this laser to in-active */
    void deactivate();

    /** @return if this laser is currently active */
    bool isActive() const;

    int getPlayer() const;

  private:
    /** Laser speed in px/second. Original game had 7px per frame (60fps). */
    static constexpr float Speed = 28 * 60;

    /** Color of all lasers (Red) */
    static inline const gfx::Color Color = gfx::Color::Red;

    /** Size of all lasers (px) */
    static inline const gfx::Vector2f Size{4.0, 24.0};

    /** Only draw active lasers. */
    bool m_active = false;

    int m_player = 0;
};
