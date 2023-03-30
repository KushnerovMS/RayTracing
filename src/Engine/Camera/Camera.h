#pragma once

#include "../../math/geometry.h"

namespace Engine {

class Camera final
{
    int width_  = 0;
    int height_ = 0;

    Vec3f dir_;
    Vec3f x_;
    Vec3f y_;

    public:

    Vec3f center;

    Camera () {}

    void            setResolution(const int width, const int height);
    int             getWidth() const { return width_; }
    int             getHeight() const { return height_; }

    void            setDirection(const Vec3f& dir);
    const Vec3f&    getDirection() const { return dir_; }

    Vec3f           getRay (int x, int y) const;
};

} // namespace Engine
