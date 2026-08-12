/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#include "Input.hpp"

namespace gfx::Keyboard
{

SDL_Scancode toScancode(const Key key)
{
    switch (key)
    {
    case Key::Return:
        return SDL_SCANCODE_RETURN;
    case Key::Escape:
        return SDL_SCANCODE_ESCAPE;
    case Key::Space:
        return SDL_SCANCODE_SPACE;
    case Key::Up:
        return SDL_SCANCODE_UP;
    case Key::Down:
        return SDL_SCANCODE_DOWN;
    case Key::Left:
        return SDL_SCANCODE_LEFT;
    case Key::Right:
        return SDL_SCANCODE_RIGHT;
    case Key::Q:
        return SDL_SCANCODE_Q;
    case Key::R:
        return SDL_SCANCODE_R;
    case Key::F:
        return SDL_SCANCODE_F;
    case Key::D:
        return SDL_SCANCODE_D;
    case Key::G:
        return SDL_SCANCODE_G;
    case Key::Num1:
        return SDL_SCANCODE_1;
    case Key::Num2:
        return SDL_SCANCODE_2;
    default:
        return SDL_SCANCODE_UNKNOWN;
    }
}

bool isKeyPressed(const Key key)
{
    const SDL_Scancode scancode = toScancode(key);
    if (scancode == SDL_SCANCODE_UNKNOWN)
    {
        return false;
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state[scancode] != 0U;
}

} // namespace gfx::Keyboard

namespace
{

gfx::Keyboard::Key fromScancode(const SDL_Scancode scancode)
{
    switch (scancode)
    {
    case SDL_SCANCODE_RETURN:
        return gfx::Keyboard::Key::Return;
    case SDL_SCANCODE_ESCAPE:
        return gfx::Keyboard::Key::Escape;
    case SDL_SCANCODE_SPACE:
        return gfx::Keyboard::Key::Space;
    case SDL_SCANCODE_UP:
        return gfx::Keyboard::Key::Up;
    case SDL_SCANCODE_DOWN:
        return gfx::Keyboard::Key::Down;
    case SDL_SCANCODE_LEFT:
        return gfx::Keyboard::Key::Left;
    case SDL_SCANCODE_RIGHT:
        return gfx::Keyboard::Key::Right;
    case SDL_SCANCODE_Q:
        return gfx::Keyboard::Key::Q;
    case SDL_SCANCODE_R:
        return gfx::Keyboard::Key::R;
    case SDL_SCANCODE_F:
        return gfx::Keyboard::Key::F;
    case SDL_SCANCODE_D:
        return gfx::Keyboard::Key::D;
    case SDL_SCANCODE_G:
        return gfx::Keyboard::Key::G;
    case SDL_SCANCODE_1:
        return gfx::Keyboard::Key::Num1;
    case SDL_SCANCODE_2:
        return gfx::Keyboard::Key::Num2;
    default:
        return gfx::Keyboard::Key::Unknown;
    }
}

} // namespace

namespace gfx
{

bool pollEvent(Event& event)
{
    event.type = Event::None;

    SDL_Event sdlEvent{};
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            event.type = Event::Closed;
            return true;
        case SDL_WINDOWEVENT:
            if (sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                event.type        = Event::Resized;
                event.size.width  = static_cast<unsigned int>(sdlEvent.window.data1);
                event.size.height = static_cast<unsigned int>(sdlEvent.window.data2);
                return true;
            }
            break;
        case SDL_KEYDOWN:
            event.type     = Event::KeyPressed;
            event.key.code = fromScancode(static_cast<SDL_Scancode>(sdlEvent.key.keysym.scancode));
            return true;
        default:
            break;
        }
    }

    return false;
}

} // namespace gfx
