/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#include <cassert>
#include "SoundManager.hpp"

SoundManager* SoundManager::m_s_Instance = nullptr;

SoundManager::SoundManager() : m_audioOn(false)
{
    // assert prevent's multiple TextureManagers for being created
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
		m_audioOn = false;
	else
		m_audioOn = true;
}

SoundManager::~SoundManager()
{
    for (const auto& [key, value] : m_audioCache)
    {
        Mix_FreeChunk(value);
    }
    // TODO: Stop looping sounds
	Mix_HaltMusic();
	Mix_CloseAudio();
}

Mix_Chunk* SoundManager::GetSound(const char *path)
{
    std::string filename{path};

    // Check mapping for the filename, return value if found (C++17 init statement syntax)
    if (auto got = m_audioCache.find(filename); got != m_audioCache.end())
    {
        return got->second;
    }
    auto sound = Mix_LoadWAV(path);
    m_audioCache[filename] = sound;
    return sound;
}

void SoundManager::Halt(int channel)
{
    Mix_HaltChannel(channel);
}

int SoundManager::Play(Mix_Chunk* sound, bool loop)
{
    return Mix_PlayChannel(-1, sound, loop ? -1 : 0);
}
