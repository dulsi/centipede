/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#pragma once

#include <SDL2/SDL.h>

#include "Types.hpp"

namespace gfx
{

class Texture
{
  public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    bool loadFromFile(const char* path);

    [[nodiscard]] Vector2f getSize() const;
    /*[[nodiscard]]*/ SDL_Texture* getGpuTexture(SDL_Renderer* renderer);

  private:
    void release();
    void ensureGpuTexture(SDL_Renderer* renderer);

    SDL_Surface* m_surface = nullptr;
    SDL_Texture* m_texture = nullptr;
    int          m_width   = 0;
    int          m_height  = 0;
};

} // namespace gfx
