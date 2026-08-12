/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
The ant class moves down making mushrooms.
*/

#pragma once
#include <random>

#include "gfx/RenderTarget.hpp"
#include "gfx/RenderWindow.hpp"
#include "gfx/Sprite.hpp"
#include "gfx/Types.hpp"

#include "Mushrooms.hpp"

class Ant : public gfx::Sprite
{
  public:
    Ant(gfx::FloatRect bounds, MushroomManager& shroomMan);
    Ant() = delete;

    void reset();
    void spawn();
    void update(float deltaTime);
    void draw(gfx::RenderTarget& target) const;

    bool checkLaserCollision(gfx::FloatRect collider);
    gfx::FloatRect getCollider() const;
    bool isDead() const;

  private:
    static constexpr int AnimationFrames = 4;

    static inline const gfx::IntRect AntAnimationOffset[AnimationFrames] =
    {
      {16, 16, 32, 35}, {64, 16, 32, 35}, {112, 16, 32, 35}, {160, 16, 32, 35}
    };
    static constexpr float Speed = 480;

    const double m_animationDuration = 0.05;
    const int m_mushroomChance = 30;
    const size_t m_spawnChance = 5;

    gfx::FloatRect m_bounds;
    std::mt19937 m_rng;
    MushroomManager& m_shroomMan;

    int m_animation;
    int m_health;
    bool m_alive = true;
    bool m_respawnNow = false;

    double m_moveTimer    = 0;
    double m_animationTimer = 0;
};
