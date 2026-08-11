/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>

#include "gfx/Texture.hpp"

struct SDL_Renderer;

/**
 * Central cache of textures.
 * Prevents loading files from disks more than once,
 * and allows many sprites to share the same texture.
 * Modified from Chapter 18 code.
 */
class TextureManager
{
  private:
    /** Pointer of the same type as the class itself
     *  the one and only instance.
     * */
    static TextureManager* m_s_Instance;

    /** Mapping of filenames to Texture objects */
    std::unordered_map<std::string, gfx::Texture> m_texCache;

    static SDL_Renderer* m_renderer;

  public:
    /**
     * Only one TextureManager should every be created.
     * Constructor stores a static class reference to the first instance.
     */
    TextureManager();

    static void SetRenderer(SDL_Renderer* renderer);

    /**
     * @brief Return a texture reference, loading it from a file if necessary
     *
     * This is a static method that makes it easy for any code to get a texture reference.
     * @param filename the texture to load
     * @return gfx::Texture&
     */
    static const gfx::Texture& GetTexture(const char* path);
};
