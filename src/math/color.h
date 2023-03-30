#pragma once

#include "cmath"
#include "assert.h"

struct Color
{
    float r = 0;
    float g = 0;
    float b = 0;

    Color() = default;
    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

          float& operator[](const size_t i)       { assert(i<3); return i<=0 ? r : (1==i ? g : b); }
    const float& operator[](const size_t i) const { assert(i<3); return i<=0 ? r : (1==i ? g : b); }

    Color& operator*=(const Color& rhs) {
        r *= rhs.r; g *= rhs.g; b *= rhs.b;
        return *this;
    }

    Color& operator+=(const Color& rhs) {
        r += rhs.r; g += rhs.g; b += rhs.b;
        return *this;
    }

    Color& operator-=(const Color& rhs) {
        r -= rhs.r; g -= rhs.g; b -= rhs.b;
        return *this;
    }

    Color& operator*=(float rhs) {
        r *= rhs; g *= rhs; b *= rhs;
        return *this;
    }

    Color& normalize() {
        r = std::max(0.0f, std::min(1.0f, r));
        g = std::max(0.0f, std::min(1.0f, g));
        b = std::max(0.0f, std::min(1.0f, b));
        return *this;
    }
};

Color operator+(Color lhs, const Color& rhs) ;

Color operator-(Color lhs, const Color& rhs) ;

Color operator*(Color lhs, const Color& rhs) ;

Color operator*(Color lhs, float rhs) ;

Color operator*(float lhs, Color rhs) ;
