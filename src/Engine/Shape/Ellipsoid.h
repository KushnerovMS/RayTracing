#pragma once

#include <memory>
#include "Interface.h"

namespace Engine {

namespace Shape {

class Ellipsoid : public Shape::Interface
{
    Vec3f center_;
    float x2c0_;
    float y2c0_;
    float z2c0_;

    float xyc0_;
    float yzc0_;
    float zxc0_;

    float x2c_;
    float y2c_;
    float z2c_;

    float xyc_;
    float yzc_;
    float zxc_;

    public:

    Ellipsoid(const Vec3f &center, float a, float b, float c) ;
    std::unique_ptr<Interface> Clone() const;


    bool    RayIntersect(const Vec3f& orig, const Vec3f& dir, float &t) const;
    Vec3f   GetN(const Vec3f& t) const;

    void    SaveState();
    void    rMove(const Vec3f& delta) { center_ += delta; }
    void     Move(const Vec3f& delta) { center_ = delta; }
    void    rScale(const Vec3f& dir);
    void     Scale(const Vec3f& dir);
    void    rRotate(float angle, const Vec3f dir);
    void     Rotate(float angle, const Vec3f dir);

    const Vec3f&    getCenter() { return center_; }
};

} // namespace Shape

} // namespace Ellipsoid
