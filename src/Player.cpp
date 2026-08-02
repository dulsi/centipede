/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Starship player character class definition.
The player can move around a given bounds using the WASD or arrow keys.
If a enemy collides with the player, a life is lost.
*/

#include "SFML/Graphics.hpp"
#include <unordered_set>
#include <ranges>

#include "Engine.hpp"
#include "Player.hpp"
#include "Laser.hpp"
#include "TextureManager.hpp"

/** Constructor initializes the Sprite and other members and sets the origin to the center. */
Player::Player(sf::FloatRect bounds, int number) : m_lastFired{sf::Time::Zero}
{
    m_number = number;
    this->setTexture(TextureManager::GetTexture("graphics/fairy.png"));
    this->setTextureRect(Player::PlayerAnimationOffset[m_number][m_animation]);

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
    m_scoreSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));

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
        m_up    = sf::Keyboard::Up;
        m_down  = sf::Keyboard::Down;
        m_left  = sf::Keyboard::Left;
        m_right = sf::Keyboard::Right;
        m_fire  = sf::Keyboard::Space;
    }
    else
    {
        m_up    = sf::Keyboard::R;
        m_down  = sf::Keyboard::F;
        m_left  = sf::Keyboard::D;
        m_right = sf::Keyboard::G;
        m_fire  = sf::Keyboard::Q;
    }

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

void Player::drawLives(sf::RenderWindow& window) const
{
    for (size_t i = 0 ; (int)(i + 1) < m_lives && i < m_lifeSprites.size(); i++)
    {
        window.draw(m_lifeSprites[(size_t)i]);
    }
}

void Player::drawScore(sf::RenderWindow& window)
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
        m_scoreSprite.setTextureRect(sf::IntRect(digit * 32, 32, 32, 32));
        m_scoreSprite.setPosition(static_cast<float>(x), 0.0f);
        window.draw(m_scoreSprite);
        value /= 10;
        x -= 32;
    }
}

void Player::disable()
{
    m_lives = 0;
}

void Player::reset()
{
    m_lastFired = sf::Time::Zero;
    // reset position
    const auto& size = this->getLocalBounds();
    sf::Vector2f start{m_bounds.left + (m_bounds.width / 2) - size.width * 1.5f + size.width * (float)m_number, // center
                       m_bounds.top + m_bounds.height};      // bottom row
    this->setPosition(start);
}

/** Set movement flags based on current keyboard input */
void Player::handleInput()
{
    // Can be moved with arrows or WASD
    m_movingUp    = sf::Keyboard::isKeyPressed(m_up);
    m_movingDown  = sf::Keyboard::isKeyPressed(m_down);
    m_movingLeft  = sf::Keyboard::isKeyPressed(m_left);
    m_movingRight = sf::Keyboard::isKeyPressed(m_right);
    m_fireLaser   = sf::Keyboard::isKeyPressed(m_fire);
}

/**
 * Move the player position according to movement flags.
 * Prevent going out of bounds.
 */
void Player::update(float deltaTime, Engine& engine)
{
    m_animationTimer += deltaTime;
    if (m_animationTimer > m_animationDuration)
    {
        m_animation++;
        m_animation %= AnimationFrames;
        m_animationTimer = 0;
        this->setTextureRect(Player::PlayerAnimationOffset[m_number][m_animation]);
    }
    // moves `Speed` pixels every second.
    // opposite directions cancel out.
    const float distance = Player::Speed * deltaTime;

    const sf::Vector2f oldPos = this->getPosition();
    sf::Vector2f       newPos  = oldPos;

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
    // not using sf::Rec.contains() because of 'sticky' walls issue
    newPos.x = saturate(newPos.x, m_bounds.left, m_bounds.width + m_bounds.left);
    newPos.y = saturate(newPos.y, m_bounds.top, m_bounds.height + m_bounds.top);

    const float dx = newPos.x - oldPos.x;
    const float dy = newPos.y - oldPos.y;

    sf::FloatRect collider = this->getGlobalBounds();
    collider.left += dx;
    collider.top += dy;

    const std::unordered_set<Engine::CollisionTarget> targets{
        Engine::CollisionTarget::Mushroom,
        m_number == 0 ? Engine::CollisionTarget::Player2 : Engine::CollisionTarget::Player1,
    };

    if (!engine.CheckCollision(collider, targets))
    {
        this->setPosition(newPos);
    }
    else if (dx != 0.f && dy != 0.f)
    {
        sf::FloatRect xRect = this->getGlobalBounds();
        xRect.left += dx;
        if (!engine.CheckCollision(xRect, targets))
        {
            this->setPosition({newPos.x, oldPos.y});
        }
        else
        {
            sf::FloatRect yRect = this->getGlobalBounds();
            yRect.top += dy;
            if (!engine.CheckCollision(yRect, targets))
            {
                this->setPosition({oldPos.x, newPos.y});
            }
        }
    }
}

/** Detect if hit by the enemy and lose a life */
bool Player::checkEnemyCollision(sf::FloatRect enemy)
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
    m_lives--;
    this->reset();
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
sf::Vector2f Player::getGunPosition() const
{
    return this->getPosition() - sf::Vector2f(0.0, this->getLocalBounds().height / 2.f);
}

sf::FloatRect Player::getCollider() const
{
    return getGlobalBounds();
}

int Player::getNumber() const
{
    return m_number;
}

bool Player::shouldFire(const sf::Time& totalGameTime)
{
    if (m_fireLaser)
    {
        auto elapsed = totalGameTime.asMilliseconds() - m_lastFired.asMilliseconds();

        // only fire after the firing period has elapsed
        if (elapsed > static_cast<int>(1000 / Laser::FireRate))
        {
            m_lastFired = totalGameTime;
            return true;
        }
    }
    return false;
}

void Player::addScore(unsigned long points)
{
    m_score += points;
}
