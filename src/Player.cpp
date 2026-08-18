/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Starship player character class definition.
The player can move around a given bounds using the WASD or arrow keys.
If a enemy collides with the player, a life is lost.
*/

#include <unordered_set>

#include "Engine.hpp"
#include "Player.hpp"
#include "Laser.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

/** Constructor initializes the Sprite and other members and sets the origin to the center. */
Player::Player(gfx::FloatRect bounds, int number) : AnimatingSprite(PlayerAnimationStates, PlayerAnimationOffset[number])
{
    m_number = number;
    this->setTexture(TextureManager::GetTexture("graphics/fairy.png"));
    setCurrentAnimation(0);

    if (const auto* texture = this->getTexture())
    {
        int index = 0;
        for (auto& lifeSprite : m_lifeSprites)
        {
            lifeSprite.setTexture(*texture);
            lifeSprite.setTextureRect(Player::PlayerAnimationOffset[m_number][1]);
            lifeSprite.setPosition((float)(m_number ? (1024 - 256 - (index * 28)) : (256 + (index * 28))), 0);
            index++;
        }
        m_lifeSprites.back().setTextureRect(Player::PlayerPlusOffset[m_number]);
    }

    m_scoreSprite.setTexture(TextureManager::GetTexture("graphics/font.png"));
    m_scoreSprite.setTextureRect(gfx::IntRect(0, 32, 32, 32));

    // use the sprite size to center the origin
    const auto& size = this->getLocalBounds();
    this->setOrigin(size.width / 2.f, size.height / 2.f);

    // adjust the bounds to account for offset sprite center
    bounds.left += size.width / 2.f;
    bounds.top += size.height / 2.f;

    bounds.width -= size.width;
    bounds.height -= size.height;
    m_bounds = bounds;

    if (number == 0)
    {
        m_up    = gfx::Keyboard::Key::Up;
        m_down  = gfx::Keyboard::Key::Down;
        m_left  = gfx::Keyboard::Key::Left;
        m_right = gfx::Keyboard::Key::Right;
        m_fire  = gfx::Keyboard::Key::Space;
    }
    else
    {
        m_up    = gfx::Keyboard::Key::R;
        m_down  = gfx::Keyboard::Key::F;
        m_left  = gfx::Keyboard::Key::D;
        m_right = gfx::Keyboard::Key::G;
        m_fire  = gfx::Keyboard::Key::Q;
    }

    m_deathSound = SoundManager::GetManager().GetSound("sounds/charmed.ogg");

    // move to starting position
    this->spawn();
}

void Player::spawn()
{
    // reset lives and position
    m_lives = Player::StartingLives;
    m_score = 0;
    this->reset();
}

void Player::drawLives(gfx::RenderWindow& window) const
{
    for (size_t i = 0 ; (int)(i + 1) < m_lives && i < m_lifeSprites.size(); i++)
    {
        window.draw(m_lifeSprites[(size_t)i]);
    }
}

void Player::drawScore(gfx::RenderWindow& window)
{
    unsigned long value = m_score;
    bool firstDigit = true;

    int x = 224;
    if (m_number == 1)
    {
        x = static_cast<int>(Game::GameSize.x) - 32;
    }
    while (value > 0 || firstDigit)
    {
        firstDigit = false;
        const int digit = static_cast<int>(value % 10);
        m_scoreSprite.setTextureRect(gfx::IntRect(digit * 32, 32, 32, 32));
        m_scoreSprite.setPosition(static_cast<float>(x), 0.0f);
        window.draw(m_scoreSprite);
        value /= 10;
        x -= 32;
    }
}

void Player::disable()
{
    m_lives = 0;
    m_score = 0;
}

void Player::reset()
{
    if (getCurrentAnimation() == 1)
    {
        m_lives--;
    }
    setCurrentAnimation(0);
    m_lastFiredMs = std::chrono::milliseconds::zero();
    // reset position
    const auto& size = this->getLocalBounds();
    gfx::Vector2f start{m_bounds.left + (m_bounds.width / 2) - size.width * 1.5f + size.width * (float)m_number, // center
                       m_bounds.top + m_bounds.height};      // bottom row
    this->setPosition(start);
}

/** Set movement flags based on current keyboard input */
void Player::handleInput()
{
    // Can be moved with arrows or WASD
    m_movingUp    = gfx::Keyboard::isKeyPressed(m_up);
    m_movingDown  = gfx::Keyboard::isKeyPressed(m_down);
    m_movingLeft  = gfx::Keyboard::isKeyPressed(m_left);
    m_movingRight = gfx::Keyboard::isKeyPressed(m_right);
    m_fireLaser   = gfx::Keyboard::isKeyPressed(m_fire);
}

/**
 * Move the player position according to movement flags.
 * Prevent going out of bounds.
 */
void Player::update(float deltaTime, Engine& engine)
{
    AnimatingSprite::update(deltaTime);
    if (engine.getState() == Engine::State::DeathReset)
    {
        return;
    }
    // moves `Speed` pixels every second.
    // opposite directions cancel out.
    const float distance = Player::Speed * deltaTime;

    const gfx::Vector2f oldPos = this->getPosition();
    gfx::Vector2f       newPos  = oldPos;

    if (m_movingUp)
    {
        newPos.y -= distance;
    }

    if (m_movingDown)
    {
        newPos.y += distance;
    }

    if (m_movingRight)
    {
        newPos.x += distance;
    }

    if (m_movingLeft)
    {
        newPos.x -= distance;
    }

    // simple inplace saturation check
    auto saturate = [](float v, float lo, float hi) { return v < lo ? lo : v > hi ? hi : v; };

    // prevent movement out of the player bounding area
    newPos.x = saturate(newPos.x, m_bounds.left, m_bounds.width + m_bounds.left);
    newPos.y = saturate(newPos.y, m_bounds.top, m_bounds.height + m_bounds.top);

    const float dx = newPos.x - oldPos.x;
    const float dy = newPos.y - oldPos.y;

    gfx::FloatRect collider = this->getGlobalBounds();
    collider.left += dx;
    collider.top += dy;

    const std::unordered_set<Engine::CollisionTarget> targets{
        Engine::CollisionTarget::Mushroom,
        m_number == 0 ? Engine::CollisionTarget::Player2 : Engine::CollisionTarget::Player1,
    };

    if (!engine.checkCollision(collider, targets))
    {
        this->setPosition(newPos);
    }
    else if (dx != 0.f && dy != 0.f)
    {
        gfx::FloatRect xRect = this->getGlobalBounds();
        xRect.left += dx;
        if (!engine.checkCollision(xRect, targets))
        {
            this->setPosition({newPos.x, oldPos.y});
        }
        else
        {
            gfx::FloatRect yRect = this->getGlobalBounds();
            yRect.top += dy;
            if (!engine.checkCollision(yRect, targets))
            {
                this->setPosition({oldPos.x, newPos.y});
            }
        }
    }
}

void Player::update(float deltaTime)
{
    AnimatingSprite::update(deltaTime);
}

/** Detect if hit by the enemy and lose a life */
bool Player::checkEnemyCollision(gfx::FloatRect enemy)
{
    if (this->getGlobalBounds().intersects(enemy))
    {
        die();
        return true;
    };
    return false;
}

void Player::die()
{
    setCurrentAnimation(1);
    SoundManager::GetManager().Play(m_deathSound);
}

void Player::extraLife()
{
    m_lives++;
}

bool Player::isDead() const
{
    return m_lives <= 0;
}

/** Calculate the offset from center origin that the top of the laster should start from. */
gfx::Vector2f Player::getGunPosition() const
{
    return this->getPosition() - gfx::Vector2f(0.0, this->getLocalBounds().height / 2.f);
}

gfx::FloatRect Player::getCollider() const
{
    return getGlobalBounds();
}

int Player::getNumber() const
{
    return m_number;
}

bool Player::shouldFire(const std::chrono::milliseconds& totalGameTimeMs)
{
    if (m_fireLaser)
    {
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(totalGameTimeMs - m_lastFiredMs).count();

        // only fire after the firing period has elapsed
        if (elapsed > static_cast<int>(1000 / Laser::FireRate))
        {
            m_lastFiredMs = totalGameTimeMs;
            return true;
        }
    }
    return false;
}

void Player::addScore(unsigned long points)
{
    m_score += points;
}
