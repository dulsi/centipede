/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#pragma once

#include <SDL2/SDL.h>

namespace gfx
{

namespace Keyboard
{

enum class Key
{
    Unknown,
    Return,
    Escape,
    Space,
    Up,
    Down,
    Left,
    Right,
    Q,
    R,
    F,
    D,
    G,
    Num1,
    Num2,
};

SDL_Scancode toScancode(Key key);
bool isKeyPressed(Key key);

} // namespace Keyboard

struct Event
{
    enum Type
    {
        None,
        Closed,
        Resized,
        KeyPressed,
    };

    Type type = None;

    struct SizeEvent
    {
        unsigned int width  = 0;
        unsigned int height = 0;
    } size;

    struct KeyEvent
    {
        Keyboard::Key code = Keyboard::Key::Unknown;
    } key;
};

bool pollEvent(Event& event);

} // namespace gfx
