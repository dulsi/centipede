/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
The Centipede class controls it's descent down the game area every update.
Multiple Centipedes may be present in the game area with different segment lengths
*/

#pragma once
#include <list>

#include "gfx/RenderTarget.hpp"
#include "gfx/RenderWindow.hpp"
#include "gfx/Sprite.hpp"
#include "gfx/Types.hpp"

#include "Mushrooms.hpp"
#include "Settings.hpp"

class Segment : public gfx::Sprite
{
  public:
    Segment(gfx::FloatRect bounds);

    Segment() = delete;

    enum class Moving { Right, Left };
    enum class Animation { None, Start, Mid1, Mid2, Final };

    void update(float deltaTime, bool updateFrame);
    void detectEdgeCollisions();
    bool detectMushroomCollisions(const Shroom& shroom);

    gfx::Vector2f getLeftEdge() const;
    gfx::Vector2f getRightEdge() const;

    void setHead();
    bool isHead() const;
    bool isAnimating();

  private:
    static constexpr int AnimationFrames = 4;

    static inline const gfx::IntRect HeadAnimationOffset[AnimationFrames] =
    {
      {208, 16, 32, 32}, {256, 16, 32, 32}, {304, 16, 32, 32}, {352, 16, 32, 32}
    };
    static inline const gfx::IntRect BodyAnimationOffset[AnimationFrames] =
    {
      {16, 16, 32, 32}, {64, 16, 32, 32}, {112, 16, 32, 32}, {160, 16, 32, 32}
    };

    Moving m_direction = Moving::Left;
    Animation m_animation = Animation::None;
    gfx::FloatRect m_bounds;

    bool m_descending = true;
    bool m_poisoned = false;
    bool m_isHead = false;

    int m_animationFrame = 0;
};

class Centipede
{
  public:
    static constexpr float Speed = Game::GridSize * 15;
    static constexpr float AnimSpeed = 8;
    static constexpr int MaxLength = 12;

    Centipede(const gfx::FloatRect& bounds, MushroomManager& shroomMan);

    Centipede(const Centipede&) = delete;
    Centipede& operator=(const Centipede&) = delete;

    void checkMushroomCollision();
    bool checkLaserCollision(gfx::FloatRect laser);
    bool checkPlayerCollision(gfx::FloatRect player);
    void update(float deltaTime);
    void draw(gfx::RenderTarget& target) const;

    void reset();
    bool isDead() const;

  private:
    const double m_animationDuration = 0.1;

    void splitAt(std::list<Segment>::iterator segment_it);

    gfx::FloatRect m_bounds;
    MushroomManager& m_shroomMan;
    std::list<Segment> m_segments;

    double m_animationTimer = 0;
};

constexpr Segment::Moving operator!(const Segment::Moving& dir) noexcept;
