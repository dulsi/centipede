/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Player character declaration.
*/

#pragma once

#include <array>
#include <cstdint>
#include <chrono>

#include "gfx/RenderWindow.hpp"
#include "gfx/Sprite.hpp"
#include "gfx/Input.hpp"
#include "gfx/Types.hpp"

class Engine;

/**
 * The Player class inherits from Sprite and implements:
 * user input, enemy collisions, and keeping track of lives
 */
class Player : public gfx::Sprite
{
  public:
    /** Construct a new Player object */
    Player(gfx::FloatRect bounds, int number);

    // no default constructor
    Player() = delete;

    /** Start the player in the middle of defined player area */
    void spawn();

    void disable();

    /** Do player movement */
    void handleInput();

    /** Update player sprite position based on elapsed seconds
     * @param deltaTime time in seconds since last update
     * @param engine game engine for collision checks
     */
    void update(float deltaTime, Engine& engine);

    void drawLives(gfx::RenderWindow& window) const;

    void drawScore(gfx::RenderWindow& window);

    /**
     * Check for collisions with enemy and decrement life counter
     * @param enemy Collider of the enemy
     * @return true if hit by enemy
     * @return false otherwise
     */
    bool checkEnemyCollision(gfx::FloatRect enemy);

    /**
     * Causes player to lose a life
     */
    void die();

    /** Move back to the starting position. */
    void reset();

    /**
     * Causes player to gain a life
     */
    void extraLife();

    /**
     * Determine if all the lives are used up.
     *
     * @return true if lives < 0
     * @return false otherwise
     */
     bool isDead() const;

    /**
     * Return the location that the lasers should spawn from.
     *
     * @return gfx::Vector2f
     */
    gfx::Vector2f getGunPosition() const;

    /**
     * Get the player collider for collisions
     *
     * @return gfx::FloatRect
     */
     gfx::FloatRect getCollider() const;

     int getNumber() const;

     bool shouldFire(const std::chrono::milliseconds& totalGameTimeMs);

     void addScore(unsigned long points);

  private:
    /** Player movement speed in pixels/second */
    static constexpr float Speed = 960;

    /** How many lives the player has at start */
    static constexpr int StartingLives = 3;

    static constexpr int AnimationFrames = 2;

    /** Location of the player texture in sprite-sheet */
    static inline const gfx::IntRect PlayerAnimationOffset[2][AnimationFrames] =
    {
      { {16, 16, 28, 32}, {64, 16, 28, 32} },
      { {16, 64, 28, 32}, {64, 64, 28, 32} },
    };

    static inline const gfx::IntRect PlayerPlusOffset[2] =
    {
      {112, 16, 28, 32},
      {112, 64, 28, 32}
    };

    /** Seconds between animation direction */
    const double m_animationDuration = 0.1;

    /** The bounds of player movement */
    gfx::FloatRect m_bounds;

    int m_number;

    gfx::Keyboard::Key m_up;
    gfx::Keyboard::Key m_down;
    gfx::Keyboard::Key m_left;
    gfx::Keyboard::Key m_right;
    gfx::Keyboard::Key m_fire;

    /** Up movement key is pressed */
    bool m_movingUp = false;
    /** Down movement key is pressed */
    bool m_movingDown = false;
    /** Left movement key is pressed */
    bool m_movingLeft = false;
    /** Right movement key is pressed */
    bool m_movingRight = false;

    bool m_fireLaser = false;

    /** Time a laser was fired */
    std::chrono::milliseconds m_lastFiredMs;

    std::array<gfx::Sprite, 5> m_lifeSprites;

    gfx::Sprite m_scoreSprite;

    int m_animation = 0;

    double m_animationTimer = 0;

    /** The current lives remaining */
    int m_lives = Player::StartingLives;

    unsigned long m_score = 0;
};
