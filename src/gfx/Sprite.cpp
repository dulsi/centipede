/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#include "Sprite.hpp"

#include "RenderWindow.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>

namespace gfx
{

void Sprite::setTexture(const Texture& texture)
{
    m_texture = &texture;
    if (m_textureRect.width == 0 && m_textureRect.height == 0)
    {
        const Vector2f size = texture.getSize();
        m_textureRect       = IntRect{0, 0, static_cast<int>(size.x), static_cast<int>(size.y)};
    }
}

const Texture* Sprite::getTexture() const
{
    return m_texture;
}

void Sprite::setTextureRect(const IntRect& rect)
{
    m_textureRect = rect;
}

IntRect Sprite::getTextureRect() const
{
    return m_textureRect;
}

Vector2f Sprite::getOrigin() const
{
    return m_origin;
}

void Sprite::setOrigin(float x, float y)
{
    m_origin = {x, y};
}

void Sprite::setPosition(float x, float y)
{
    m_position = {x, y};
}

void Sprite::setPosition(const Vector2f& position)
{
    m_position = position;
}

void Sprite::move(float dx, float dy)
{
    m_position.x += dx;
    m_position.y += dy;
}

void Sprite::setScale(const Vector2f& scale)
{
    m_scale = scale;
}

Vector2f Sprite::getScale() const
{
    return m_scale;
}

Vector2f Sprite::getPosition() const
{
    return m_position;
}

FloatRect Sprite::getLocalBounds() const
{
    return FloatRect{
        0.f,
        0.f,
        static_cast<float>(m_textureRect.width),
        static_cast<float>(m_textureRect.height),
    };
}

FloatRect Sprite::getGlobalBounds() const
{
    const float absScaleX = std::fabs(m_scale.x);
    const float absScaleY = std::fabs(m_scale.y);
    const float width     = static_cast<float>(m_textureRect.width) * absScaleX;
    const float height    = static_cast<float>(m_textureRect.height) * absScaleY;
    const float left      = m_position.x - (m_origin.x * absScaleX);
    const float top       = m_position.y - (m_origin.y * absScaleY);

    return FloatRect{left, top, width, height};
}

void Sprite::draw(RenderTarget& target) const
{
    if (getTexture() == nullptr)
    {
        return;
    }

    SDL_Texture* texture = const_cast<Texture*>(getTexture())->getGpuTexture(target.getRenderer());
    if (texture == nullptr)
    {
        return;
    }

    const IntRect  sourceRect = getTextureRect();
    const Vector2f scale      = getScale();
    const Vector2f position   = getPosition();
    const Vector2f origin     = getOrigin();

    const float absScaleX = std::fabs(scale.x);
    const float absScaleY = std::fabs(scale.y);
    const float destW     = static_cast<float>(sourceRect.width) * absScaleX;
    const float destH     = static_cast<float>(sourceRect.height) * absScaleY;

    const SDL_Rect src{
        sourceRect.left,
        sourceRect.top,
        sourceRect.width,
        sourceRect.height,
    };

    const SDL_FRect dst{
        position.x - (origin.x * absScaleX),
        position.y - (origin.y * absScaleY),
        destW,
        destH,
    };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (scale.x < 0.f)
    {
        flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
    }
    if (scale.y < 0.f)
    {
        flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
    }

    SDL_RenderCopyExF(target.getRenderer(), texture, &src, &dst, 0.0, nullptr, flip);
}

} // namespace gfx
