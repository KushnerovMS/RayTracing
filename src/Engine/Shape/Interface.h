#pragma once

#include "memory"
#include "../../math/geometry.h"


namespace Engine {

namespace Shape {

class Interface
{
    public:

    virtual ~Interface() = default;

    virtual std::unique_ptr<Interface> Clone() const = 0;

    virtual bool            RayIntersect(const Vec3f& orig, const Vec3f& dir, float& t) const = 0;
    virtual Vec3f           GetN(const Vec3f& t) const = 0;

    virtual void            SaveState() = 0;
    virtual void            rMove(const Vec3f& delta) = 0;
    virtual void             Move(const Vec3f& delta) = 0;
    virtual void            rScale(const Vec3f& dir) = 0;
    virtual void             Scale(const Vec3f& dir) = 0;
    virtual void            rRotate(float angle, const Vec3f dir) = 0;
    virtual void             Rotate(float angle, const Vec3f dir) = 0;

    virtual const Vec3f&    getCenter() = 0;

};

} // namespace Shape

} // namespace Engine
