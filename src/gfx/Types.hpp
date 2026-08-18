/*
SPDX-License-Identifier: BSD-3-Clause
Copyright (c) 2024 Jackson Miller
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace gfx
{

struct Vector2f
{
    float x = 0.f;
    float y = 0.f;

    Vector2f() = default;
    Vector2f(float xVal, float yVal) : x{xVal}, y{yVal} {}

    Vector2f operator+(const Vector2f& other) const { return {x + other.x, y + other.y}; }
    Vector2f operator-(const Vector2f& other) const { return {x - other.x, y - other.y}; }
    Vector2f operator*(float scalar) const { return {x * scalar, y * scalar}; }

    Vector2f& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
};

inline Vector2f operator/(Vector2f vec, float scalar)
{
    vec /= scalar;
    return vec;
}

struct Vector2i
{
    int x = 0;
    int y = 0;

    Vector2i() = default;
    Vector2i(int xVal, int yVal) : x{xVal}, y{yVal} {}
};

struct IntRect
{
    int left   = 0;
    int top    = 0;
    int width  = 0;
    int height = 0;

    IntRect() = default;
    IntRect(int leftVal, int topVal, int widthVal, int heightVal)
        : left{leftVal}, top{topVal}, width{widthVal}, height{heightVal}
    {
    }
};

struct FloatRect
{
    float left   = 0.f;
    float top    = 0.f;
    float width  = 0.f;
    float height = 0.f;

    FloatRect() = default;
    FloatRect(float leftVal, float topVal, float widthVal, float heightVal)
        : left{leftVal}, top{topVal}, width{widthVal}, height{heightVal}
    {
    }

    Vector2f getSize() const { return {width, height}; }

    bool intersects(const FloatRect& other) const
    {
        const float aLeft   = left;
        const float aTop    = top;
        const float aRight  = left + width;
        const float aBottom = top + height;

        const float bLeft   = other.left;
        const float bTop    = other.top;
        const float bRight  = other.left + other.width;
        const float bBottom = other.top + other.height;

        return (aLeft < bRight) && (aRight > bLeft) && (aTop < bBottom) && (aBottom > bTop);
    }
};

struct Color
{
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 255;

    Color() = default;
    constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255)
        : r{red}, g{green}, b{blue}, a{alpha}
    {
    }

    static const Color Black;
    static const Color Red;
    static const Color White;
    static const Color Transparent;
};

inline const Color Color::Black = {0, 0, 0};
inline const Color Color::Red = {255, 0, 0};
inline const Color Color::White = {255, 255, 255};
inline const Color Color::Transparent = {0, 0, 0, 0};

} // namespace gfx
