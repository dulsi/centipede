/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
The ant class moves down making mushrooms.
*/

#pragma once
#include <random>

#include <SFML/Graphics.hpp>

#include "Mushrooms.hpp"

class Ant : public sf::Drawable
{
  public:
    /** Construct a new Ant object that moves within `bounds` */
    Ant(sf::FloatRect bounds, MushroomManager& shroomMan);
    // no default constructor
    Ant() = delete;

    void reset();

    /** Set the starting position and state */
    void spawn();

    /** Update the ant's movement based on elapsed time */
    void update(float deltaTime);

    /** Draw only living ants to the target
     * Sprite overload
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /** Check if a laser hit this ant, and 'kill' it
     * @return true if the ant was hit.
     */
    bool checkLaserCollision(sf::FloatRect collider);

    /**
     * Get the ant collider for collisions
     *
     * @return sf::FloatRect
     */
    sf::FloatRect getCollider() const;

    bool isDead() const;

  private:
    static constexpr int AnimationFrames = 4;

    /** The location of the scorpion texture in the sprite-sheet */
    static inline const sf::IntRect AntAnimationOffset[AnimationFrames] =
    {
      {16, 16, 32, 35}, {64, 16, 32, 35}, {112, 16, 32, 35}, {160, 16, 32, 35}
    };
    /** Speed of movement in px/s for both x and y components */
    static constexpr float Speed = 480;

    /** Seconds between changing animation */
    const double m_animationDuration = 0.05;

    /** Chance of making a mushroom */
    const int m_mushroomChance = 30;

    const size_t m_spawnChance = 5;

    /** The scorpion sprite */
    sf::Sprite m_sprite;

    /** The area the scorpion can move in */
    sf::FloatRect m_bounds;

    /** Random number generator for erratic movement */
    std::mt19937 m_rng;

    /** Reference to the mushrooms so we can create more.
     * Aggregate member (not owned).
     */
    MushroomManager& m_shroomMan;

    int m_animation;

    int m_health;

    /** If the ant is alive or note */
    bool m_alive = true;

    bool m_respawnNow = false;

    // A bunch of properties for controlling the scorpion movement state-machine
    double m_moveTimer    = 0;
    double m_animationTimer = 0;
};
