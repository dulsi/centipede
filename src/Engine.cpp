/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Defines the main game Engine and game loop logic.
*/
#include <iostream>
#include <chrono>

#include "Engine.hpp"
#include "Settings.hpp"

/**
 * Construct a new Engine:: Engine object
 *
 * Initializer list handles creating member objects.
 * Body sets window and view settings
 */
Engine::Engine(gfx::RenderWindow& window)
    : m_window(window),
      texMan(),
      m_player{{Game::PlayerArea, 0}, {Game::PlayerArea, 1}},
      m_shroomMan{Game::ShroomArea},
      m_centipede{Game::EnemyArea, m_shroomMan},
      m_ant{Game::EnemyArea, m_shroomMan},
      m_spider{Game::SpiderArea},
      m_scorpion{Game::ScorpionArea},
      m_totalGameTime{std::chrono::milliseconds::zero()}
{
    // made my own startup image
    m_startSprite.setTexture(TextureManager::GetTexture("graphics/splash.png"));
}

/**
 * Main entry-point into the game loop.
 * Calls the input-update-draw methods until the window is closed.
 */
void Engine::run()
{
    // reset the clock for first run
    m_lastTime = std::chrono::steady_clock::now();
    while (m_window.isOpen())
    {
        constexpr float tick = 1 / 60.f;
        const auto now = std::chrono::steady_clock::now();
        const auto delta = now - m_lastTime;
        if (std::chrono::duration<double>(delta).count() < tick)
        {
            SDL_Delay(static_cast<Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(tick - std::chrono::duration<double>(delta).count())).count()));
        }
        m_lastTime = std::chrono::steady_clock::now();
        m_totalGameTime += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(tick));

        // only draw frames at a maximum tick rate
        input();
        update(tick);
        draw();
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
    gfx::Event event;
    while (m_window.pollEvent(event))
    {

        // Close the window when "X" button clicked
        if (event.type == gfx::Event::Closed)
        {
            m_window.close();
        }

        // preserve the aspect ratio when resizing
        /*if (event.type == sf::Event::Resized)
        {
            this->setViewport(event.size.width, event.size.height);
        }*/

        if (event.type == gfx::Event::KeyPressed)
        {

            // Start game from "menu" with "ENTER"
            if (state == State::Start &&
                (event.key.code == gfx::Keyboard::Key::Return ||
                    event.key.code == gfx::Keyboard::Key::Space ||
                    event.key.code == gfx::Keyboard::Key::Q ||
                    event.key.code == gfx::Keyboard::Key::Num1 ||
                    event.key.code == gfx::Keyboard::Key::Num2))
            {
                int players = 1;

                if (event.key.code == gfx::Keyboard::Key::Q || event.key.code == gfx::Keyboard::Key::Num2)
                {
                    players = 2;
                }

                state = State::Playing;
                std::cout << "Started" << std::endl;
                //m_clock.restart(); // restart clock to prevent frame skip
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
            if (event.key.code == gfx::Keyboard::Key::Escape)
            {
                std::cout << "Ended" << std::endl;
                m_window.close();
            }
        }
    } // end event polling

    if (gfx::Keyboard::isKeyPressed(gfx::Keyboard::Key::Num1) && gfx::Keyboard::isKeyPressed(gfx::Keyboard::Key::Num2))
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
    if (state == State::DeathReset)
    {
        m_stateTimer += dtSeconds;
        if (m_stateTimer >= m_stateDuration)
        {
            for (auto& player : m_player)
            {
                player.reset();
            }
            for (auto& laser : m_lasers)
            {
                laser.deactivate();
            }
            m_spider.reset();
            m_scorpion.reset();
            m_ant.reset();
            m_centipede.reset();
            state = State::Playing;
        }
        return;
    }
    if ((state != State::Playing) && (state != State::LevelChange))
    {
        return;
    }

    if (!m_spider.isDead())
    {
        m_shroomMan.checkSpiderCollision(m_spider.getCollider());
    }

    if (!m_scorpion.isDead())
    {
        m_shroomMan.checkScorpionCollision(m_scorpion.getCollider());
    }

    for (auto& player : m_player)
    {
        if (player.isDead())
        {
            continue;
        }
        if (!m_spider.isDead())
        {
            if (player.checkEnemyCollision(m_spider.getCollider()))
            {
                state = State::DeathReset;
                m_stateTimer = 0;
                continue;
            }
        }
        if (!m_ant.isDead())
        {
            if (player.checkEnemyCollision(m_ant.getCollider()))
            {
                state = State::DeathReset;
                m_stateTimer = 0;
                continue;
            }
        }

        if (m_centipede.checkPlayerCollision(player.getCollider()))
        {
            player.die();
            state = State::DeathReset;
            m_stateTimer = 0;
            continue;
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
    else if ((state == State::Playing) || (state == State::LevelChange) || (state == State::DeathReset))
    {
        // draw all the objects during game-play
        m_window.draw(m_shroomMan);

        m_window.draw(m_ant);

        m_window.draw(m_spider);

        m_window.draw(m_scorpion);

        // draw centipede(s)
        m_window.draw(m_centipede);

        if (state != State::DeathReset)
        {
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
                }
                player.drawScore(m_window);
            }
        }
    }

    m_window.display();
}

bool Engine::CheckCollision(const gfx::FloatRect& rect,
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
