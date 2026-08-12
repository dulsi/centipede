/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
The spider class moves around the play area clearing mushrooms.
If the player hits the spider, a life is lost and the player is reset.
*/

#pragma once
#include <random>

#include "gfx/RenderWindow.hpp"
#include "gfx/Sprite.hpp"
#include "gfx/Types.hpp"

class Spider : public gfx::Sprite
{
  public:
    Spider(gfx::FloatRect bounds);
    Spider() = delete;

    void reset();
    void spawn();
    void update(float deltaTime);
    void draw(SDL_Renderer* target) const;

    bool checkLaserCollision(gfx::FloatRect collider);
    gfx::FloatRect getCollider() const;
    bool isDead() const;

    enum class Moving { Up, Down, UpRight, UpLeft, DownLeft, DownRight };

  private:
    static constexpr int AnimationFrames = 4;

    static inline const gfx::IntRect SpiderAnimationOffset[AnimationFrames] =
    {
      {16, 16, 60, 32}, {96, 16, 60, 32}, {176, 16, 60, 32}, {256, 16, 60, 32}
    };
    static constexpr float Speed = 240;

    const double m_moveDuration = 0.5;
    const double m_animationDuration = 0.05;
    const double m_respawnDuration = 5;

    gfx::FloatRect m_bounds;
    std::mt19937 m_rng;

    Moving m_direction;
    int m_animation;
    bool m_alive = true;

    double m_moveTimer    = 0;
    double m_animationTimer = 0;
    double m_respawnTimer = 0;

    bool m_moveLeft = false;
};
