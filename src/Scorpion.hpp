/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne

Description:
The scorpion class moves across the field poisoning mushrooms.
*/

#pragma once
#include <random>

#include <SFML/Graphics.hpp>

class Scorpion : public sf::Drawable
{
  public:
    /** Construct a new Spider object that moves within `bounds` */
    Scorpion(sf::FloatRect bounds);
    // no default constructor
    Scorpion() = delete;

    void reset();

    /** Set the starting position and state */
    void spawn();

    /** Update the scorpion's movement based on elapsed time */
    void update(float deltaTime);

    /** Draw only living scorpions to the target
     * Sprite overload
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /** Check if a laser hit this scorpion, and 'kill' it
     * @return true if the spider was hit.
     */
    bool checkLaserCollision(sf::FloatRect collider);

    /**
     * Get the scorpion collider for collisions
     *
     * @return sf::FloatRect
     */
    sf::FloatRect getCollider() const;

    bool isDead() const;

  private:
    static constexpr int AnimationFrames = 4;

    /** The location of the scorpion texture in the sprite-sheet */
    static inline const sf::IntRect ScorpionAnimationOffset[AnimationFrames] =
    {
      {16, 16, 60, 32}, {96, 16, 60, 32}, {176, 16, 60, 32}, {256, 16, 60, 32}
    };
    /** Speed of movement in px/s for both x and y components */
    static constexpr float Speed = 240;

    /** Seconds between changing animation */
    const double m_animationDuration = 0.05;

    /** Seconds to wait before re-spawning */
    const double m_respawnDuration = 5;

    const size_t m_spawnChance = 5;

    /** The scorpion sprite */
    sf::Sprite m_sprite;

    /** The area the scorpion can move in */
    sf::FloatRect m_bounds;

    /** Random number generator for erratic movement */
    std::mt19937 m_rng;

    int m_animation;

    /** If the scorpion is alive or note */
    bool m_alive = true;

    // A bunch of properties for controlling the scorpion movement state-machine
    double m_moveTimer    = 0;
    double m_animationTimer = 0;
    double m_respawnTimer = 0;
};
