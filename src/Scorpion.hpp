/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
The scorpion class moves across the field poisoning mushrooms.
*/

#pragma once
#include <random>

#include "gfx/RenderTarget.hpp"
#include "gfx/RenderWindow.hpp"
#include "gfx/Sprite.hpp"
#include "gfx/Types.hpp"

class Scorpion : public gfx::Sprite
{
  public:
    Scorpion(gfx::FloatRect bounds);
    Scorpion() = delete;

    void reset();
    void spawn();
    void update(float deltaTime);
    void draw(gfx::RenderTarget& target) const;

    bool checkLaserCollision(gfx::FloatRect collider);
    gfx::FloatRect getCollider() const;
    bool isDead() const;

  private:
    static constexpr int AnimationFrames = 4;

    static inline const gfx::IntRect ScorpionAnimationOffset[AnimationFrames] =
    {
      {16, 16, 60, 32}, {96, 16, 60, 32}, {176, 16, 60, 32}, {256, 16, 60, 32}
    };
    static constexpr float Speed = 240;

    const double m_animationDuration = 0.05;
    const double m_respawnDuration = 5;
    const size_t m_spawnChance = 5;

    gfx::FloatRect m_bounds;
    std::mt19937 m_rng;

    int m_animation;
    bool m_alive = true;

    double m_moveTimer    = 0;
    double m_animationTimer = 0;
    double m_respawnTimer = 0;
};
