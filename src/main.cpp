/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller

Description:
Centipede Game using C++ and SFML.
*/
#include "Engine.hpp"
#include <cstring>

int main(int argc, char *argv[])
{
    bool fullscreen = true;
	for (int i = 1; i < argc; i++)
	{
		if (!std::strcmp(argv[i], "-w"))
        {
            fullscreen = false;
        }
    }
    try
    {
        gfx::RenderWindow window;

        // create the window at native game resolution
        if (!window.create(
            static_cast<unsigned>(Game::GameSize.x), static_cast<unsigned>(Game::GameSize.y),
            Game::Name,
            fullscreen))
        {
            throw std::runtime_error("Failed to create SDL window");
        }
        TextureManager::SetRenderer(window.getRenderer());
        Engine engine(window);
        engine.run();
    }
    catch (const std::exception& e)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
