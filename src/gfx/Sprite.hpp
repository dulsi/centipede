/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#pragma once

#include "Drawable.hpp"
#include "Texture.hpp"
#include "Types.hpp"

namespace gfx
{

class Sprite : public Drawable
{
  public:
    void setTexture(const Texture& texture);
    const Texture* getTexture() const;

    void setTextureRect(const IntRect& rect);
    IntRect getTextureRect() const;
    Vector2f getOrigin() const;
    void setOrigin(float x, float y);
    void setPosition(float x, float y);
    void setPosition(const Vector2f& position);
    void move(float dx, float dy);
    void setScale(const Vector2f& scale);
    Vector2f getScale() const;
    Vector2f getPosition() const;
    FloatRect getLocalBounds() const;
    FloatRect getGlobalBounds() const;

    void draw(SDL_Renderer* target) const;

  private:
    const Texture* m_texture     = nullptr;
    IntRect        m_textureRect{};
    Vector2f       m_position{};
    Vector2f       m_origin{};
    Vector2f       m_scale{1.f, 1.f};
};

} // namespace gfx
