/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#pragma once

#include "Input.hpp"
#include "RectangleShape.hpp"
#include "RenderTarget.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

struct SDL_Renderer;
struct SDL_Window;

class Ant;
class Centipede;
class Laser;
class MushroomManager;
class Scorpion;
class Spider;

namespace gfx
{

class RenderWindow
{
  public:
    RenderWindow() = default;
    ~RenderWindow();

    RenderWindow(const RenderWindow&)            = delete;
    RenderWindow& operator=(const RenderWindow&) = delete;

    bool create(unsigned int width, unsigned int height, const char* title, bool fullscreen);
    [[nodiscard]] bool isOpen() const;
    void close();
    [[nodiscard]] bool pollEvent(Event& event);
    void clear(const Color& color);
    void display();
    void setPosition(int x, int y);
    [[nodiscard]] SDL_Renderer* getRenderer() const;

    void draw(const Sprite& sprite);
    void draw(const RectangleShape& shape);
    void draw(const MushroomManager& manager);
    void draw(const Centipede& centipede);
    void draw(const Spider& spider);
    void draw(const Scorpion& scorpion);
    void draw(const Ant& ant);
    void draw(const Laser& laser);

  private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool          m_open     = false;
    unsigned int  m_logicalWidth  = 0;
    unsigned int  m_logicalHeight = 0;
};

} // namespace gfx
