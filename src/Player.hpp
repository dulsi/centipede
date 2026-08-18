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
#include <vector>

#include "gfx/RenderWindow.hpp"
#include "gfx/Input.hpp"
#include "gfx/Types.hpp"
#include "AnimatingSprite.hpp"
#include "SoundManager.hpp"

class Engine;

/**
 * The Player class inherits from Sprite and implements:
 * user input, enemy collisions, and keeping track of lives
 */
class Player : public AnimatingSprite
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
    void update(float deltaTime);

    /** Player movement speed in pixels/second */
    static constexpr float Speed = 960;

    /** How many lives the player has at start */
    static constexpr int StartingLives = 3;

    static inline const std::vector<AnimationStateInfo> PlayerAnimationStates =
    {
      { 0.1, {0, 1}, true },
      { 0.4, {2, 3, 4, 5, 6}, false },
    };

    /** Location of the player texture in sprite-sheet */
    static inline const std::vector<gfx::IntRect> PlayerAnimationOffset[2] =
    {
      { {16, 16, 28, 32}, {64, 16, 28, 32}, {160, 16, 28, 32}, {208, 16, 28, 32}, {256, 16, 28, 32}, {304, 16, 28, 32}, {336, 0, 28, 32} },
      { {16, 64, 28, 32}, {64, 64, 28, 32}, {160, 64, 28, 32}, {208, 64, 28, 32}, {256, 64, 28, 32}, {304, 64, 28, 32}, {336, 0, 28, 32} },
    };

    static inline const gfx::IntRect PlayerPlusOffset[2] =
    {
      {112, 16, 28, 32},
      {112, 64, 28, 32}
    };

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

    /** The current lives remaining */
    int m_lives = Player::StartingLives;

    unsigned long m_score = 0;

    Mix_Chunk* m_deathSound;
};
