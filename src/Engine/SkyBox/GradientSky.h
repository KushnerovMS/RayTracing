#pragma once

#include "SkyBox.h"

namespace Engine {

namespace SkyBox {

class Gradient : public SkyBox::Interface
{
    public:

    Color color1;
    Color color2;
    Vec3f direction;

    Gradient(Color beg   = Color(1, 1, 1),
             Color end   = Color(1, 1, 1),
             Vec3f dir   = Vec3f(0, 1, 0)) :
        color1( (beg.normalize() + end.normalize()) * 0.5f ),
        color2( ( end.normalize() - beg.normalize() ) * 0.5f ), direction(dir) {}

    Color getColor(const Vec3f& dir) const { return color1 + color2 * (dir * direction); }
};

} // namespace SkyBox

} // namespace Engine
