/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Declare the Game Engine. Engine::run() is the main entrypoint into the game.
*/

#pragma once
#include <array>
#include <unordered_set>
#include <chrono>

#include "gfx/RenderWindow.hpp"
#include "gfx/Types.hpp"

#include "Ant.hpp"
#include "Centipede.hpp"
#include "Laser.hpp"
#include "Mushrooms.hpp"
#include "Player.hpp"
#include "Scorpion.hpp"
#include "Spider.hpp"
#include "TextureManager.hpp"

/**
 * The Engine is responsible for:
 *  - setting up the game window,
 *  - main event loop,
 *  - getting user input,
 *  - updating game objects,
 *  - drawing to the frame
 */
class Engine
{
  public:
    /** Construct a new Engine object */
    Engine(gfx::RenderWindow& window);
    /** Create a window and run the entire game loop */
    void run();

    /**
     * Used to control the game loop state-machine
     */
    enum class State { Start, Playing, LevelChange, DeathReset, GameOver };

    enum class CollisionTarget { Mushroom, Player1, Player2 };

    /**
     * Check if a rect collides with any of the specified targets.
     * @param rect The collider to test
     * @param targets Which object types to check against
     * @return true if any target intersects rect
     */
    bool CheckCollision(const gfx::FloatRect& rect,
                        const std::unordered_set<CollisionTarget>& targets) const;

  private:
    const double m_stateDuration = 1;

    /** Color for the game world background */
    static inline const gfx::Color WorldColor = gfx::Color::Black;

    /** The game RenderWindow */
    gfx::RenderWindow& m_window;

    /** A cache for all textures used on game sprites.
     * default constructor sets up a static pointer to the only instance.
     */
    const TextureManager texMan;

    /** The player-controlled starship */
    Player m_player[2];

    /** Manager for all the mushrooms in the scene */
    MushroomManager m_shroomMan;

    /** All the centipedes on the screen */
    Centipede m_centipede;

    /** The ant antagonist makes mushrooms */
    Ant m_ant;

    /** The spider antagonist moves randomly and clears mushrooms */
    Spider m_spider;

    /** The scorpion antagonist poisons mushrooms */
    Scorpion m_scorpion;

    /** A pool of 30 laser objects to recycle (should be plenty) */
    std::array<Laser, 30> m_lasers;

    size_t m_currentLaser = 0;

    /** Start/Game over screen sprite */
    gfx::Sprite m_startSprite;

    /** Game state machine */
    State state = State::Start;

    std::chrono::steady_clock::time_point m_lastTime;
    std::chrono::milliseconds m_totalGameTime = std::chrono::milliseconds::zero();
    double m_stateTimer = 0;

    /** Poll player input and hand-off to objects */
    void input();

    /** Update all game objects in the scene (and detect collisions) */
    void update(const float dtAsSeconds);

    /** Draw all objects the the frame-buffer */
    void draw();
};
