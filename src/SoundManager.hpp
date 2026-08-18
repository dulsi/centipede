/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#pragma once

#include <unordered_map>
#include <string>
#include <SDL2/SDL_mixer.h>

class SoundManager
{
  public:
    SoundManager();
    ~SoundManager();

    Mix_Chunk* GetSound(const char *path);

    void Halt(int channel);

    int Play(Mix_Chunk* sound, bool loop = false);

    static SoundManager& GetManager() { return *m_s_Instance; }

  private:
    bool m_audioOn;

    /** Mapping of filenames to sound data */
    std::unordered_map<std::string, Mix_Chunk*> m_audioCache;

    static SoundManager* m_s_Instance;
};
