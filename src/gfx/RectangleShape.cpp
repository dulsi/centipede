/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#include "RectangleShape.hpp"

#include "RenderWindow.hpp"

namespace gfx
{

RectangleShape::RectangleShape(const Vector2f& size) : m_size{size} {}

void RectangleShape::setFillColor(const Color& color)
{
    m_color = color;
}

void RectangleShape::setOrigin(float x, float y)
{
    m_origin = {x, y};
}

void RectangleShape::setPosition(float x, float y)
{
    m_position = {x, y};
}

void RectangleShape::setPosition(const Vector2f& position)
{
    m_position = position;
}

void RectangleShape::move(float dx, float dy)
{
    m_position.x += dx;
    m_position.y += dy;
}

Vector2f RectangleShape::getPosition() const
{
    return m_position;
}

Color RectangleShape::getFillColor() const
{
    return m_color;
}

FloatRect RectangleShape::getGlobalBounds() const
{
    const float left = m_position.x - m_origin.x;
    const float top  = m_position.y - m_origin.y;
    return FloatRect{left, top, m_size.x, m_size.y};
}

void RectangleShape::draw(SDL_Renderer* target) const
{
    const FloatRect bounds = getGlobalBounds();
    const SDL_FRect rect{bounds.left, bounds.top, bounds.width, bounds.height};
    const Color     color = getFillColor();
    SDL_SetRenderDrawColor(target, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(target, &rect);
}

} // namespace gfx
