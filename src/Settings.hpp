/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Settings and Constants for the Centipede game
*/

#pragma once
#include <SFML/Graphics.hpp>

namespace Game
{

/** Name of the game for window title, debug, etc. */
inline constexpr auto& Name = "Centipede";

/** Play area is made up of 8x8 px grid */
inline constexpr int GridSize = 32.0;

/** Main play area is 30x30 grid, with an extra row on top and bottom. */
inline const sf::Vector2f GameSize{960, 1024};

/** The view is centered so origin is top left (0,0). */
inline const sf::Vector2f GameCenter{GameSize / 2.0f};

/** The area Centipede can move in */
static inline const sf::FloatRect EnemyArea{0, Game::GridSize, Game::GameSize.x, Game::GameSize.y - 2 * Game::GridSize};

/** The area the spider can move in */
static inline const sf::FloatRect SpiderArea{0, Game::GridSize * 16, Game::GameSize.x, Game::GridSize * 15};

/** The area the scorpion can move in */
static inline const sf::FloatRect ScorpionArea{0, Game::GridSize, Game::GameSize.x, Game::GameSize.y - 5 * Game::GridSize};

/** The area mushrooms spawn in */
static inline const sf::FloatRect ShroomArea{0, Game::GridSize * 4, Game::GameSize.x, Game::GameSize.y - Game::GridSize * 6};

/** The area player can move in (bottom 4 rows) */
static inline const sf::FloatRect PlayerArea{0, Game::GameSize.y - Game::GridSize * 5, Game::GameSize.x, Game::GridSize * 4};

}; // end namespace Game
