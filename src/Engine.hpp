/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Declare the Game Engine. Engine::run() is the main entrypoint into the game.
*/

#pragma once
#include <array>
#include <unordered_set>

#include "SFML/Graphics.hpp"

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
    Engine();
    /** Create a window and run the entire game loop */
    void run();

    /**
     * Used to control the game loop state-machine
     */
    enum class State { Start, Playing, LevelChange, GameOver };

    enum class CollisionTarget { Mushroom, Player1, Player2 };

    /**
     * Check if a rect collides with any of the specified targets.
     * @param rect The collider to test
     * @param targets Which object types to check against
     * @return true if any target intersects rect
     */
    bool CheckCollision(const sf::FloatRect& rect,
                        const std::unordered_set<CollisionTarget>& targets) const;

  private:
    /** Color for the game world background */
    static inline const sf::Color WorldColor = sf::Color::Black;

    /** A cache for all textures used on game sprites.
     * default constructor sets up a static pointer to the only instance.
     */
    const TextureManager texMan;

    /** The game RenderWindow */
    sf::RenderWindow m_window;

    /** The game view, sized to match Game::GameSize */
    sf::View m_view;

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
    sf::Sprite m_startSprite;

    /** Visual border for the edge of the game */
    sf::RectangleShape m_border;

    /** Game state machine */
    State state = State::Start;

    /** Main game clock */
    sf::Clock m_clock;

    /** Elapsed game time */
    sf::Time m_totalGameTime;

    /** Elapsed game time */
    double m_elapsedTime = 0;

    /** Poll player input and hand-off to objects */
    void input();

    /** Update all game objects in the scene (and detect collisions) */
    void update(const float dtAsSeconds);

    /** Draw all objects the the frame-buffer */
    void draw();

    /** Resize the viewport to preserve the game aspect ratio when the window is resized
     * @param width, height new size of the main window
     */
    void setViewport(unsigned int width, unsigned int height);
};
