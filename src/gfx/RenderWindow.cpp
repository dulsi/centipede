/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#include "RenderWindow.hpp"

#include "../Centipede.hpp"
#include "../Mushrooms.hpp"

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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
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

void RenderWindow::draw(const Drawable& sprite)
{
    if (m_renderer == nullptr)
    {
        return;
    }
    sprite.draw(m_renderer);
}

} // namespace gfx
