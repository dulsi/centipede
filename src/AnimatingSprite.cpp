/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2026 Dennis Payne
*/

#include "AnimatingSprite.hpp"

AnimatingSprite::AnimatingSprite(const std::vector<AnimationStateInfo>& states, const std::vector<gfx::IntRect>& offsets)
: m_states(states), m_offsets(offsets)
{
}

void AnimatingSprite::update(float deltaTime)
{
    m_animationTimer += deltaTime;
    if (m_animationTimer > m_states[m_animationCurrent].duration)
    {
        if (m_states[m_animationCurrent].frames.size() - 1 > m_animationState)
        {
            m_animationState++;
        }
        else if (m_states[m_animationCurrent].loop == true)
        {
            m_animationState = 0;
        }
        m_animationTimer = 0;
        setTextureRect(m_offsets[m_states[m_animationCurrent].frames[m_animationState]]);
    }
}

void AnimatingSprite::setCurrentAnimation(size_t current)
{
    m_animationCurrent = current;
    m_animationState = 0;
    setTextureRect(m_offsets[m_states[m_animationCurrent].frames[m_animationState]]);
}
