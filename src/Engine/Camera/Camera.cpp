#include "Camera.h"

void Engine::Camera::setResolution(const int width, const int height)
{
    if (width_ == 0)    width_ = width;
    if (height_ == 0)   height_ = height;
}

void Engine::Camera::setDirection(const Vec3f& dir)
{
    dir_ = dir;
    x_ = cross(Vec3f(0, 1, 0), dir_).normalize();
    y_ = (float(height_) / float(width_)) * cross(x_, dir_).normalize();
}

Vec3f Engine::Camera::getRay(int x, int y) const
{
    return (dir_ + float(2.0 * x / float(width_) - 1.f) * x_
                 + float(2.0 * y / float(height_) - 1.f) * y_).normalize();
}
