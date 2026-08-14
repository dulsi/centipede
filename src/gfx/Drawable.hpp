/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#pragma once

struct SDL_Renderer;

namespace gfx
{

class Drawable
{
  public:
    virtual ~Drawable();

    virtual void draw(SDL_Renderer* target) const = 0;
};

} // namespace gfx
