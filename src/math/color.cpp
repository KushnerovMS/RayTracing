#include "color.h"

Color operator+(Color lhs, const Color& rhs) {
    return lhs += rhs;
}

Color operator-(Color lhs, const Color& rhs) {
    return lhs -= rhs;
}

Color operator*(Color lhs, const Color& rhs) {
    return lhs *= rhs;
}

Color operator*(Color lhs, float rhs) {
    return lhs *= rhs;
}

Color operator*(float lhs, Color rhs) {
    return rhs *= lhs;
}
