/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#include <cassert>
#include <exception>

#include <SDL2/SDL_image.h>

#include "TextureManager.hpp"

TextureManager* TextureManager::m_s_Instance = nullptr;
SDL_Renderer*   TextureManager::m_renderer   = nullptr;

/** Constructor sets up the static reference. */
TextureManager::TextureManager() : m_texCache()
{
    // assert prevent's multiple TextureManagers for being created
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;

    const int flags = IMG_INIT_PNG;
    if ((IMG_Init(flags) & flags) == 0)
    {
        throw std::runtime_error("Could not initialize SDL_image");
    }
}

void TextureManager::SetRenderer(SDL_Renderer* renderer)
{
    m_renderer = renderer;
}

/**
 * @brief Return a texture reference, loading it from a file if necessary
 *
 * This is a static method that makes it easy for any code to get a texture reference.
 * @param path the texture to load
 * @return gfx::Texture&
 */
const gfx::Texture& TextureManager::GetTexture(const char* path)
{
    // convert to string
    std::string filename{path};
    // reference to mapping in instance object
    auto& texture_cache = m_s_Instance->m_texCache;

    // Check mapping for the filename, return value if found (C++17 init statement syntax)
    if (auto got = texture_cache.find(filename); got != texture_cache.end())
    {
        if (m_renderer != nullptr)
        {
            got->second.getGpuTexture(m_renderer);
        }
        return got->second;
    }

    // File not loaded yet!
    auto& texture = texture_cache[filename];

    if (!texture.loadFromFile(filename.c_str()))
    {
        // If file can't be found, abort
        throw std::runtime_error("Could not load file: " + filename);
    }

    if (m_renderer != nullptr)
    {
        texture.getGpuTexture(m_renderer);
    }

    return texture;
}
