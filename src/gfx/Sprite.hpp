/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#pragma once

#include "RenderTarget.hpp"
#include "Texture.hpp"
#include "Types.hpp"

namespace gfx
{

class Sprite
{
  public:
    void setTexture(const Texture& texture);
    [[nodiscard]] const Texture* getTexture() const;

    void setTextureRect(const IntRect& rect);
    [[nodiscard]] IntRect getTextureRect() const;
    [[nodiscard]] Vector2f getOrigin() const;
    void setOrigin(float x, float y);
    void setPosition(float x, float y);
    void setPosition(const Vector2f& position);
    void move(float dx, float dy);
    void setScale(const Vector2f& scale);
    [[nodiscard]] Vector2f getScale() const;
    [[nodiscard]] Vector2f getPosition() const;
    [[nodiscard]] FloatRect getLocalBounds() const;
    [[nodiscard]] FloatRect getGlobalBounds() const;

    void draw(RenderTarget& target) const;

  private:
    const Texture* m_texture     = nullptr;
    IntRect        m_textureRect{};
    Vector2f       m_position{};
    Vector2f       m_origin{};
    Vector2f       m_scale{1.f, 1.f};
};

} // namespace gfx
