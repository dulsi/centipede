/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Centipede class definition.
*/

#include <list>

#include "Centipede.hpp"
#include "Mushrooms.hpp"
#include "Settings.hpp"
#include "TextureManager.hpp"

Centipede::Centipede(const gfx::FloatRect& bounds, MushroomManager& shroomMan) : m_bounds{bounds}, m_shroomMan{shroomMan}
{
    reset();
}

void Centipede::update(float deltaTime)
{
    bool updateFrame = false;
    m_animationTimer += deltaTime;
    if (m_animationTimer >= m_animationDuration)
    {
        updateFrame = true;
        m_animationTimer = 0;
    }

    for (auto& seg : m_segments)
    {
        this->checkMushroomCollision();
        seg.update(deltaTime, updateFrame);
    }
}

void Centipede::draw(SDL_Renderer* target) const
{
    for (const auto& seg : m_segments)
    {
        if (!seg.isHead())
        {
            seg.draw(target);
        }
    }
    for (const auto& seg : m_segments)
    {
        if (seg.isHead())
        {
            seg.draw(target);
        }
    }
}

void Centipede::reset()
{
    gfx::Vector2f startPos{m_bounds.left + (m_bounds.width / 2.f), m_bounds.top + Game::GridSize / 2.f};
    m_segments.clear();

    for (int i = 0; i < Centipede::MaxLength; i++)
    {
        auto&       new_seg = m_segments.emplace_back(m_bounds);
        const float spacing = static_cast<float>(Game::GridSize * i);
        new_seg.setPosition(startPos.x + spacing, startPos.y);
    }

    m_segments.front().setHead();
}

bool Centipede::isDead() const
{
    return m_segments.size() == 0;
}

void Centipede::checkMushroomCollision()
{
    for (auto& seg : m_segments)
    {
        if (seg.isAnimating())
        {
            continue;
        }

        for (const auto& shroom : m_shroomMan.getShrooms())
        {
            seg.detectMushroomCollisions(shroom);
        }
    }
}

bool Centipede::checkLaserCollision(gfx::FloatRect laser)
{
    for (auto seg = m_segments.begin(); seg != m_segments.end(); seg++)
    {
        if (laser.intersects(seg->getGlobalBounds()))
        {
            this->splitAt(seg);
            return true;
        }
    }

    return false;
}

bool Centipede::checkPlayerCollision(gfx::FloatRect player)
{
    for (auto seg = m_segments.begin(); seg != m_segments.end(); seg++)
    {
        if (player.intersects(seg->getGlobalBounds()))
        {
            return true;
        }
    }

    return false;
}

void Centipede::splitAt(std::list<Segment>::iterator seg_it)
{
    m_shroomMan.addMushroom(seg_it->getPosition());
    auto next = m_segments.erase(seg_it);

    if (next == m_segments.end() || next->isHead())
    {
        return;
    }
    next->setHead();
    this->checkMushroomCollision();
}

Segment::Segment(gfx::FloatRect bounds) : m_bounds{bounds}
{
    const auto& tex = TextureManager::GetTexture("graphics/centipede.png");
    this->setTexture(tex);
    this->setTextureRect(Segment::BodyAnimationOffset[m_animationFrame]);

    const auto& size = this->getLocalBounds().getSize();
    this->setOrigin(size.x / 2.f, size.y / 2.f);
}

void Segment::update(float deltaTime, bool updateFrame)
{
    this->detectEdgeCollisions();

    const float distance = Centipede::Speed * deltaTime;

    if (updateFrame)
    {
        m_animationFrame++;
        m_animationFrame %= AnimationFrames;
        if (isHead())
        {
            this->setTextureRect(Segment::HeadAnimationOffset[m_animationFrame]);
        }
        else
        {
            this->setTextureRect(Segment::BodyAnimationOffset[m_animationFrame]);
        }
    }

    if (m_animation == Animation::None)
    {
        if (m_poisoned)
        {
            m_animation = Animation::Start;
        }
        else
        {
            switch (m_direction)
            {
            case Moving::Right:
                this->move(distance, 0.0);
                break;
            case Moving::Left:
                this->move(-distance, 0.0);
                break;
            };
        }
    }

    const float yDisp = m_descending ? Centipede::AnimSpeed : -Centipede::AnimSpeed;
    const float xDisp = m_direction == Moving::Right ? Centipede::AnimSpeed : -Centipede::AnimSpeed;

    switch (m_animation)
    {
    case Animation::Start:
        this->move(xDisp, yDisp);
        m_animation = Animation::Mid1;
        break;
    case Animation::Mid1:
        this->move(xDisp, yDisp);
        m_animation = Animation::Mid2;
        m_direction = !m_direction;
        break;
    case Animation::Mid2:
        this->move(xDisp, yDisp);
        m_animation = Animation::Final;
        break;
    case Animation::Final:
    {
        this->move(xDisp, yDisp);
        m_animation = Animation::None;
        auto scale = getScale();
        scale.x *= -1;
        setScale(scale);
        break;
    }
    case Animation::None:
        break;
    }
}

void Segment::detectEdgeCollisions()
{
    const float spacing = 12.0;

    static const float width     = this->getLocalBounds().width;
    const auto&        centerPos = this->getPosition();

    if (m_animation != Animation::None)
    {
        return;
    }

    if (m_direction == Moving::Right)
    {
        if ((m_bounds.left + m_bounds.width) - this->getRightEdge().x <= spacing)
        {
            m_animation = Animation::Start;
        }
    }
    else if (m_direction == Moving::Left)
    {
        if (this->getLeftEdge().x - m_bounds.left <= spacing)
        {
            m_animation = Animation::Start;
        }
    }

    if (m_descending)
    {
        if ((m_bounds.top + m_bounds.height) == (centerPos.y + width / 2))
        {
            m_descending = false;
            m_poisoned = false;
        }
    }
    else
    {
        if ((centerPos.y - width / 2) == m_bounds.top + m_bounds.height - Game::GridSize * 4)
        {
            m_descending = true;
        }
    }
}

bool Segment::detectMushroomCollisions(const Shroom& shroom)
{
    const float spacing = 12.0;

    const gfx::Vector2f& segLeft  = this->getLeftEdge();
    const gfx::Vector2f& segRight = this->getRightEdge();

    const gfx::Vector2f& shroomLeft  = shroom.getLeftEdge();
    const gfx::Vector2f& shroomRight = shroom.getRightEdge();

    if (segLeft.y != shroomLeft.y)
    {
        return false;
    }

    if (m_direction == Moving::Right && shroomLeft.x >= segRight.x)
    {
        if (shroomLeft.x - segRight.x <= spacing)
        {
            if (shroom.isPoisoned())
            {
                m_poisoned = true;
            }
            m_animation = Segment::Animation::Start;
            return true;
        }
    }
    else if (m_direction == Segment::Moving::Left && shroomRight.x <= segLeft.x)
    {
        if (segLeft.x - shroomRight.x <= spacing)
        {
            if (shroom.isPoisoned())
            {
                m_poisoned = true;
            }
            m_animation = Segment::Animation::Start;
            return true;
        }
    }

    return false;
}

gfx::Vector2f Segment::getRightEdge() const
{
    const float         width  = this->getLocalBounds().width;
    const gfx::Vector2f& center = this->getPosition();
    return gfx::Vector2f(center.x + width / 2.f, center.y);
}

gfx::Vector2f Segment::getLeftEdge() const
{
    const float         width  = this->getLocalBounds().width;
    const gfx::Vector2f& center = this->getPosition();
    return gfx::Vector2f(center.x - width / 2.f, center.y);
}

void Segment::setHead()
{
    this->setTextureRect(Segment::HeadAnimationOffset[m_animationFrame]);
    m_isHead = true;
}

bool Segment::isHead() const
{
    return m_isHead;
}

bool Segment::isAnimating()
{
    return m_animation != Animation::None;
}

constexpr Segment::Moving operator!(const Segment::Moving& d) noexcept
{
    return d == Segment::Moving::Right ? Segment::Moving::Left : Segment::Moving::Right;
}
