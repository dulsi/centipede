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

#include <SFML/Graphics.hpp>

class Shroom : public sf::Sprite
{
  public:
    /**
     * Construct a new Shroom centered at postion (x,y)
     * @param type type of mushroom
     * @param x coordinate of center
     * @param y coordinate of center
     */
    Shroom(int type, float x, float y);
    Shroom(int type, sf::Vector2f location);
    Shroom() = delete; // no default constructor

    /** Decrement the health of this mushroom, and cycle through the textures.
     *  @return remaining health
     */
    int damage();

    /**
     * Get the x,y position of the left side in world space.
     * The y position is simply the center of the mushroom sprite.
     * @return sf::Vector2f (x,y)
     */
    sf::Vector2f getLeftEdge() const;

    /**
     * Get the x,y position of the right side in world space.
     * The y position is simply the center of the mushroom sprite.
     * @return sf::Vector2f (x,y)
     */
    sf::Vector2f getRightEdge() const;

    void setType(int type);

    bool isPoisoned() const;

    void poison();

  private:
    // Constant regions for mushroom textures in the sprite-sheet
    static inline const sf::IntRect NormalTexOffset[3][4] = 
    {
      {{16, 16, 32, 32}, {64, 16, 32, 32}, {112, 16, 32, 32}, {160, 16, 32, 32}},
      {{16, 112, 32, 32}, {64, 112, 32, 32}, {112, 112, 32, 32}, {160, 112, 32, 32}},
      {{16, 208, 32, 32}, {64, 208, 32, 32}, {112, 208, 32, 32}, {160, 208, 32, 32}}
    };
    static inline const sf::IntRect PoisonTexOffset[3][4] = 
    {
      {{16, 64, 32, 32}, {64, 64, 32, 32}, {112, 64, 32, 32}, {160, 64, 32, 32}},
      {{16, 160, 32, 32}, {64, 160, 32, 32}, {112, 160, 32, 32}, {160, 160, 32, 32}},
      {{16, 256, 32, 32}, {64, 256, 32, 32}, {112, 256, 32, 32}, {160, 256, 32, 32}}
    };
    static inline const sf::IntRect Damage1TexOffset;
    static inline const sf::IntRect Damage2TexOffset;
    static inline const sf::IntRect Damage3TexOffset;

    int m_type;

    bool m_poisoned = false;

    /** The health of mushroom (starts at 4) */
    int m_health = 4;
};

/**
 * MushroomManager is used to operate on all mushroom sprites in the scene.
 * Each mushroom starts with the same texture.
 */
class MushroomManager : public sf::Drawable
{
  public:
    /** Construct the Mushroom Manager object
     * and create a bunch of mushrooms with random positions
     */
    MushroomManager(sf::FloatRect bounds);
    MushroomManager() = delete; // no default constructor

    /**
     * Draw all active mushrooms to the scene
     * Implements sf::Drawable.draw
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /**
     * Add a new mushroom to the collection
     *
     * @param location coordinate of top-left corner
     */
    void addMushroom(sf::Vector2f location);

    /**
     * Checks for a spider colliding with any mushroom.
     * Immediately removes the mushroom if hit
     *
     * @param spider The spider collider
     * @return true if the spider hit a mushroom
     */
    bool checkSpiderCollision(sf::FloatRect spider);

    /**
     * Checks for a scorpion colliding with any mushroom.
     * Immediately poisons the mushroom if hit
     *
     * @param scorpion The scorpion collider
     * @return true if the spider hit a mushroom
     */
    bool checkScorpionCollision(sf::FloatRect scorpion);

    /**
     * Checks for a laser colliding with any mushroom.
     *
     * @param laser The laser collider
     * @return true if the laser hit a mushroom
     */
    bool checkLaserCollision(sf::FloatRect laser);

    /**
     * Get a reference to the list of mushroom sprites for easy iteration
     * @return read-only reference to the internal list
     */
    const std::list<Shroom>& getShrooms() const;

    void nextLevel();

    /** Update mushrooms for next level */
    bool update(float deltaTime);

    void reset();

  private:
    /** Seconds to wait before updating level */
    const double m_levelChangeDuration = 1;

    /** Collection of mushroom sprites that this class manages */
    std::list<Shroom> m_shrooms;

    /** Area where mushroom can be placed */
    sf::FloatRect m_bounds;

    /** Mersenne twister random number engine (for random positioning) */
    std::mt19937 m_rng;

    int m_type = 0;
    double m_levelChangeTimer = 0;
};
