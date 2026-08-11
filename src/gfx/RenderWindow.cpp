/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#include "RenderWindow.hpp"

#include "../Ant.hpp"
#include "../Centipede.hpp"
#include "../Laser.hpp"
#include "../Mushrooms.hpp"
#include "../Scorpion.hpp"
#include "../Spider.hpp"

#include <SDL2/SDL.h>

#include <cmath>

namespace gfx
{

RenderWindow::~RenderWindow()
{
    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool RenderWindow::create(const unsigned int width, const unsigned int height, const char* title, const bool fullscreen)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return false;
    }

    const Uint32 flags = fullscreen ? (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN) : SDL_WINDOW_SHOWN;
    m_window           = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          static_cast<int>(width), static_cast<int>(height), flags);
    if (m_window == nullptr)
    {
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        return false;
    }

    m_logicalWidth  = width;
    m_logicalHeight = height;
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(m_renderer, static_cast<int>(width), static_cast<int>(height));
    SDL_ShowCursor(SDL_DISABLE);
    m_open = true;
    return true;
}

bool RenderWindow::isOpen() const
{
    return m_open;
}

void RenderWindow::close()
{
    m_open = false;
}

bool RenderWindow::pollEvent(Event& event)
{
    return gfx::pollEvent(event);
}

void RenderWindow::clear(const Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

void RenderWindow::display()
{
    SDL_RenderPresent(m_renderer);
}

void RenderWindow::setPosition(const int x, const int y)
{
    if (m_window != nullptr)
    {
        SDL_SetWindowPosition(m_window, x, y);
    }
}

SDL_Renderer* RenderWindow::getRenderer() const
{
    return m_renderer;
}

void RenderWindow::draw(const Sprite& sprite)
{
    if (m_renderer == nullptr || sprite.getTexture() == nullptr)
    {
        return;
    }

    SDL_Texture* texture = const_cast<Texture*>(sprite.getTexture())->getGpuTexture(m_renderer);
    if (texture == nullptr)
    {
        return;
    }

    const IntRect  sourceRect = sprite.getTextureRect();
    const Vector2f scale      = sprite.getScale();
    const Vector2f position   = sprite.getPosition();
    const Vector2f origin     = sprite.getOrigin();

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

    SDL_RenderCopyExF(m_renderer, texture, &src, &dst, 0.0, nullptr, flip);
}

void RenderWindow::draw(const RectangleShape& shape)
{
    if (m_renderer == nullptr)
    {
        return;
    }

    const FloatRect bounds = shape.getGlobalBounds();
    const SDL_FRect rect{bounds.left, bounds.top, bounds.width, bounds.height};
    const Color     color = shape.getFillColor();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(m_renderer, &rect);
}

void RenderWindow::draw(const MushroomManager& manager)
{
    manager.draw(*this);
}

void RenderWindow::draw(const Centipede& centipede)
{
    centipede.draw(*this);
}

void RenderWindow::draw(const Spider& spider)
{
    spider.draw(*this);
}

void RenderWindow::draw(const Scorpion& scorpion)
{
    scorpion.draw(*this);
}

void RenderWindow::draw(const Ant& ant)
{
    ant.draw(*this);
}

void RenderWindow::draw(const Laser& laser)
{
    laser.draw(*this);
}

} // namespace gfx
