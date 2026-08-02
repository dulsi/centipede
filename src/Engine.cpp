/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Defines the main game Engine and game loop logic.
*/
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Engine.hpp"
#include "Settings.hpp"

/**
 * Construct a new Engine:: Engine object
 *
 * Initializer list handles creating member objects.
 * Body sets window and view settings
 */
Engine::Engine()
    : texMan(),
      m_view{Game::GameCenter, Game::GameSize},
      m_player{{Game::PlayerArea, 0}, {Game::PlayerArea, 1}},
      m_shroomMan{Game::ShroomArea},
      m_centipede{Game::EnemyArea, m_shroomMan},
      m_ant{Game::EnemyArea, m_shroomMan},
      m_spider{Game::SpiderArea},
      m_scorpion{Game::ScorpionArea},
      m_totalGameTime{sf::Time::Zero}
{

    // create the window at native game resolution
    m_window.create(
        sf::VideoMode(static_cast<unsigned>(Game::GameSize.x), static_cast<unsigned>(Game::GameSize.y)),
        Game::Name,
        sf::Style::Fullscreen);

    // set some OS window options
    m_window.setMouseCursorVisible(false);
    m_window.setFramerateLimit(60); // original game was 60fps
    m_window.setVerticalSyncEnabled(false);

    // place the window in the center of the desktop
    const auto& desktop = sf::VideoMode::getDesktopMode();
    const auto    xpos    = (desktop.width / 2u) - (static_cast<unsigned>(Game::GameSize.x) / 2u);
    const auto    ypos    = (desktop.height / 2u) - (static_cast<unsigned>(Game::GameSize.y) / 2u);
    m_window.setPosition(sf::Vector2i(static_cast<int>(xpos), static_cast<int>(ypos)));

    m_window.setView(m_view);

    // made my own startup image
    m_startSprite.setTexture(TextureManager::GetTexture("graphics/splash.png"));

    // later as a HUD overlay (not submitted)
    // m_border.setSize(Game::GameSize);
    // m_border.setOutlineThickness(-2);
    // m_border.setFillColor(sf::Color::Transparent);
    // m_border.setOutlineColor(sf::Color::White);
}

/**
 * Main entry-point into the game loop.
 * Calls the input-update-draw methods until the window is closed.
 */
void Engine::run()
{
    // reset the clock for first run
    m_clock.restart();
    while (m_window.isOpen())
    {
        const sf::Time& dt = m_clock.restart();
        m_totalGameTime += dt;
        m_elapsedTime += dt.asSeconds();
        constexpr float tick = 1 / 60.f;

        // only draw frames at a maximum tick rate
        input();
        if (m_elapsedTime >= tick)
        {
            update(tick);
            draw();
            m_elapsedTime = 0;
        }
    }
}

/**
 * Handle event input (start/stop/quit),
 * player movement input (from Player::handleInput()),
 * as well as shooting lasers with <SPACE>.
 */
void Engine::input()
{
    // handle event polling for some inputs (start/end, etc)
    sf::Event event;
    while (m_window.pollEvent(event))
    {

        // Close the window when "X" button clicked
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        // preserve the aspect ratio when resizing
        if (event.type == sf::Event::Resized)
        {
            this->setViewport(event.size.width, event.size.height);
        }

        if (event.type == sf::Event::KeyPressed)
        {

            // Start game from "menu" with "ENTER"
            if (state == State::Start &&
                (event.key.code == sf::Keyboard::Return ||
                    event.key.code == sf::Keyboard::Space ||
                    event.key.code == sf::Keyboard::Q ||
                    event.key.code == sf::Keyboard::Num1 ||
                    event.key.code == sf::Keyboard::Num2))
            {
                int players = 1;

                if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Num2)
                {
                    players = 2;
                }

                state = State::Playing;
                std::cout << "Started" << std::endl;
                m_clock.restart(); // restart clock to prevent frame skip
                m_shroomMan.reset();
                m_centipede.reset();
                for (auto& laser : m_lasers)
                {
                    laser.deactivate();
                }
                m_ant.reset();
                m_spider.reset();
                m_scorpion.reset();

                m_player[0].spawn(); // respawn the player if they are dead
                if (players == 2)
                {
                    m_player[1].spawn();
                }
                else
                {
                    m_player[1].disable();
                }
            }

            // Quit game whenever "ESC" pressed
            if (event.key.code == sf::Keyboard::Escape)
            {
                std::cout << "Ended" << std::endl;
                m_window.close();
            }
        }
    } // end event polling

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        m_window.close();
    }
    // Keyboard polling for smooth player movement
    if ((state == State::Playing) || (state == State::LevelChange))
    {

        for (auto& player : m_player)
        {
            if (player.isDead())
            {
                continue;
            }

            // Handle player movement with WASD keys
            player.handleInput();

            // Handle shooting lasers (TODO: move to Player (?) probably)
            if (player.shouldFire(m_totalGameTime))
            {
                m_lasers[m_currentLaser].shoot(player.getGunPosition(), player.getNumber());
                m_currentLaser++;
                if (m_currentLaser >= m_lasers.size())
                {
                    m_currentLaser = 0;
                }
            }
        }
    } // end input while playing
}

/**
 * Update all object positions and check for collisions
 * Check for GameOver event (player dead)
 * @param dtSeconds time since last frame
 */
void Engine::update(const float dtSeconds)
{
    // only update during the actual game
    if ((state != State::Playing) && (state != State::LevelChange))
    {
        return;
    }

    m_shroomMan.checkSpiderCollision(m_spider.getCollider());

    m_shroomMan.checkScorpionCollision(m_scorpion.getCollider());

    for (auto& player : m_player)
    {
        if (player.isDead())
        {
            continue;
        }
        if (!m_spider.isDead())
        {
            player.checkEnemyCollision(m_spider.getCollider());
        }
        if (!m_ant.isDead())
        {
            player.checkEnemyCollision(m_ant.getCollider());
        }

        if (m_centipede.checkPlayerCollision(player.getCollider()))
        {
            player.die();
        }
    }

    for (auto& laser : m_lasers)
    {
        // skip updating or colliding with inactive lasers
        if (!laser.isActive())
        {
            continue;
        }

        if (m_spider.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            continue;
        }

        if (m_scorpion.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            continue;
        }

        if (m_ant.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            if (m_ant.isDead())
            {
                m_player[laser.getPlayer()].addScore(200);
            }
            continue;
        }

        if (m_shroomMan.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            m_player[laser.getPlayer()].addScore(1);
            continue;
        }

        if (m_centipede.checkLaserCollision(laser.getCollider()))
        {
            laser.deactivate();
            m_player[laser.getPlayer()].addScore(10);
            continue;
        }

        laser.update(dtSeconds); // move the laser upward
    }

    m_centipede.update(dtSeconds);
    m_ant.update(dtSeconds);
    m_spider.update(dtSeconds);
    m_scorpion.update(dtSeconds);
    for (auto& player : m_player)
    {
        if (!player.isDead())
        {
            player.update(dtSeconds, *this);
        }
    }

    if (state == State::LevelChange)
    {
        if (m_shroomMan.update(dtSeconds))
        {
            state = State::Playing;
            m_centipede.reset();
        }
    }
    else if (m_centipede.isDead())
    {
        state = State::LevelChange;
        m_shroomMan.nextLevel();
    }

    // when the player dies, restart the game
    if ((m_player[0].isDead()) && (m_player[1].isDead()))
    {
        state = State::Start;
    }
}

/** Draw all game objects to the window.
 *
 * Implements the double buffering sequence of clear-draw-display from SFML.
 */
void Engine::draw()
{

    m_window.clear(Engine::WorldColor);

    if (state == State::Start)
    {
        // draw the start screen at beginning
        m_window.draw(m_startSprite);
    }
    else if ((state == State::Playing) || (state == State::LevelChange))
    {
        // draw all the objects during game-play
        m_window.draw(m_shroomMan);

        m_window.draw(m_ant);

        m_window.draw(m_spider);

        m_window.draw(m_scorpion);

        // draw centipede(s)
        m_window.draw(m_centipede);

        // draw lasers (automatically doesn't draw inactive ones)
        for (const auto& laser : m_lasers)
        {
            m_window.draw(laser);
        }

        for (auto& player : m_player)
        {
            if (!player.isDead())
            {
                m_window.draw(player);
                player.drawLives(m_window);
                player.drawScore(m_window);
            }
        }
    }

    m_window.display();
}

/**
 * Handles resizing the SFML viewport to preserve the correct game aspect ratio
 * when the main window is resized. This prevents any distortion of the game characters,
 * while enlarging (or shrinking) uniformly.
 *
 * @param width, height new size of the main window
 */
void Engine::setViewport(unsigned int width, unsigned int height)
{
    float windowRatio = static_cast<float>(width) / static_cast<float>(height);
    float viewRatio   = m_view.getSize().x / m_view.getSize().y;
    float sizeX       = 1;
    float sizeY       = 1;
    float posX        = 0;
    float posY        = 0;

    if (windowRatio > viewRatio)
    {
        sizeX = viewRatio / windowRatio;
        posX  = (1 - sizeX) / 2;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY  = (1 - sizeY) / 2;
    }

    m_view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    m_window.setView(m_view);
}

bool Engine::CheckCollision(const sf::FloatRect& rect,
                              const std::unordered_set<CollisionTarget>& targets) const
{
    if (targets.count(CollisionTarget::Mushroom) != 0U)
    {
        for (const auto& shroom : m_shroomMan.getShrooms())
        {
            if (rect.intersects(shroom.getGlobalBounds()))
            {
                return true;
            }
        }
    }

    if (targets.count(CollisionTarget::Player1) != 0U && !m_player[0].isDead())
    {
        if (rect.intersects(m_player[0].getCollider()))
        {
            return true;
        }
    }

    if (targets.count(CollisionTarget::Player2) != 0U && !m_player[1].isDead())
    {
        if (rect.intersects(m_player[1].getCollider()))
        {
            return true;
        }
    }

    return false;
}
