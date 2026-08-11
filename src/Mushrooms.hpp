/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
The MushroomManager and Shroom class declaration.
It creates a collection of Shroom objects, and handles their state throughout the game.
*/

#pragma once
#include <list>
#include <random>

#include "gfx/RenderTarget.hpp"
#include "gfx/RenderWindow.hpp"
#include "gfx/Sprite.hpp"
#include "gfx/Types.hpp"

class Shroom : public gfx::Sprite
{
  public:
    Shroom(int type, float x, float y);
    Shroom(int type, gfx::Vector2f location);
    Shroom() = delete;

    int damage();

    gfx::Vector2f getLeftEdge() const;
    gfx::Vector2f getRightEdge() const;

    void setType(int type);

    bool isPoisoned() const;

    void poison();

  private:
    static inline const gfx::IntRect NormalTexOffset[3][4] =
    {
      {{16, 16, 32, 32}, {64, 16, 32, 32}, {112, 16, 32, 32}, {160, 16, 32, 32}},
      {{16, 112, 32, 32}, {64, 112, 32, 32}, {112, 112, 32, 32}, {160, 112, 32, 32}},
      {{16, 208, 32, 32}, {64, 208, 32, 32}, {112, 208, 32, 32}, {160, 208, 32, 32}}
    };
    static inline const gfx::IntRect PoisonTexOffset[3][4] =
    {
      {{16, 64, 32, 32}, {64, 64, 32, 32}, {112, 64, 32, 32}, {160, 64, 32, 32}},
      {{16, 160, 32, 32}, {64, 160, 32, 32}, {112, 160, 32, 32}, {160, 160, 32, 32}},
      {{16, 256, 32, 32}, {64, 256, 32, 32}, {112, 256, 32, 32}, {160, 256, 32, 32}}
    };
    static inline const gfx::IntRect Damage1TexOffset;
    static inline const gfx::IntRect Damage2TexOffset;
    static inline const gfx::IntRect Damage3TexOffset;

    int m_type;

    bool m_poisoned = false;

    int m_health = 4;
};

class MushroomManager
{
  public:
    MushroomManager(gfx::FloatRect bounds);
    MushroomManager() = delete;

    void draw(gfx::RenderTarget& target) const;

    void addMushroom(gfx::Vector2f location);

    bool checkSpiderCollision(gfx::FloatRect spider);
    bool checkScorpionCollision(gfx::FloatRect scorpion);
    bool checkLaserCollision(gfx::FloatRect laser);

    const std::list<Shroom>& getShrooms() const;

    void nextLevel();
    bool update(float deltaTime);
    void reset();

  private:
    const double m_levelChangeDuration = 1;

    std::list<Shroom> m_shrooms;

    gfx::FloatRect m_bounds;

    std::mt19937 m_rng;

    int m_type = 0;
    double m_levelChangeTimer = 0;
};
