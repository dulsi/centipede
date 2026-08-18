/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#pragma once

#include <vector>
#include "gfx/Sprite.hpp"

struct AnimationStateInfo
{
    double duration;
    std::vector<size_t> frames;
    bool loop;
};

class AnimatingSprite : public gfx::Sprite
{
  public:
    AnimatingSprite(const std::vector<AnimationStateInfo>& states, const std::vector<gfx::IntRect>& offsets);

    /** Update sprite animation based on elapsed seconds
     * @param deltaTime time in seconds since last update
     */
    virtual void update(float deltaTime);

    size_t getCurrentAnimation() { return m_animationCurrent; }

    void setCurrentAnimation(size_t current);

  private:
    const std::vector<AnimationStateInfo>& m_states;
    const std::vector<gfx::IntRect>& m_offsets;

    size_t m_animationCurrent = 0;
    size_t m_animationState = 0;

    double m_animationTimer = 0;
};
