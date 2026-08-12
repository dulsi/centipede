/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#pragma once

#include "Drawable.hpp"
#include "Types.hpp"

struct SDL_Renderer;

namespace gfx
{

class RectangleShape : public Drawable
{
  public:
    explicit RectangleShape(const Vector2f& size);

    void setFillColor(const Color& color);
    void setOrigin(float x, float y);
    void setPosition(float x, float y);
    void setPosition(const Vector2f& position);
    void move(float dx, float dy);
    [[nodiscard]] Vector2f getPosition() const;
    [[nodiscard]] Color getFillColor() const;
    [[nodiscard]] FloatRect getGlobalBounds() const;

    void draw(SDL_Renderer* target) const;

  private:
    Vector2f  m_size;
    Color     m_color = Color::Red;
    Vector2f  m_position{};
    Vector2f  m_origin{};
};

} // namespace gfx
