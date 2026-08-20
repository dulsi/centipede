/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
The ant class moves down making mushrooms.
*/

#pragma once
#include <random>

#include "gfx/RenderWindow.hpp"
#include "gfx/Types.hpp"
#include "AnimatingSprite.hpp"

#include "Mushrooms.hpp"

class Ant : public AnimatingSprite
{
  public:
    Ant(gfx::FloatRect bounds, MushroomManager& shroomMan);
    Ant() = delete;

    void reset();
    void spawn();
    void update(float deltaTime);
    void draw(SDL_Renderer* target) const;

    bool checkLaserCollision(gfx::FloatRect collider);
    gfx::FloatRect getCollider() const;
    bool isDead() const;

  private:
    static inline const std::vector<AnimationStateInfo> AntAnimationStates =
    {
      { 0.05, {0, 1, 2, 3}, true }
    };
    static inline const std::vector<gfx::IntRect> AntAnimationOffset =
    {
      {16, 16, 32, 35}, {64, 16, 32, 35}, {112, 16, 32, 35}, {160, 16, 32, 35}
    };
    static constexpr float Speed = 480;

    const int m_mushroomChance = 30;
    const size_t m_spawnChance = 5;

    gfx::FloatRect m_bounds;
    std::mt19937 m_rng;
    MushroomManager& m_shroomMan;

    int m_health;
    bool m_alive = true;
    bool m_respawnNow = false;

    double m_moveTimer    = 0;
};
