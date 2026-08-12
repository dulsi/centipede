/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#include "Texture.hpp"

#include <SDL2/SDL_image.h>

#include <stdexcept>
#include <string>
#include <utility>

namespace gfx
{

Texture::~Texture()
{
    release();
}

Texture::Texture(Texture&& other) noexcept
    : m_surface{other.m_surface},
      m_texture{other.m_texture},
      m_width{other.m_width},
      m_height{other.m_height}
{
    other.m_surface = nullptr;
    other.m_texture = nullptr;
    other.m_width   = 0;
    other.m_height  = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        release();
        m_surface       = other.m_surface;
        m_texture       = other.m_texture;
        m_width         = other.m_width;
        m_height        = other.m_height;
        other.m_surface = nullptr;
        other.m_texture = nullptr;
        other.m_width   = 0;
        other.m_height  = 0;
    }
    return *this;
}

void Texture::release()
{
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    if (m_surface != nullptr)
    {
        SDL_FreeSurface(m_surface);
        m_surface = nullptr;
    }
}

bool Texture::loadFromFile(const char* path)
{
    release();

    m_surface = IMG_Load(path);
    if (m_surface == nullptr)
    {
        return false;
    }

    m_width  = m_surface->w;
    m_height = m_surface->h;
    return true;
}

Vector2f Texture::getSize() const
{
    return {static_cast<float>(m_width), static_cast<float>(m_height)};
}

void Texture::ensureGpuTexture(SDL_Renderer* renderer)
{
    if (m_texture != nullptr || m_surface == nullptr || renderer == nullptr)
    {
        return;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
    if (m_texture == nullptr)
    {
        throw std::runtime_error(std::string{"Failed to create texture: "} + SDL_GetError());
    }

    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(m_surface);
    m_surface = nullptr;
}

SDL_Texture* Texture::getGpuTexture(SDL_Renderer* renderer)
{
    ensureGpuTexture(renderer);
    return m_texture;
}

} // namespace gfx
