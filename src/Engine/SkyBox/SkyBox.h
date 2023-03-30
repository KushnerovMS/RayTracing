#pragma once

#include "../../math/geometry.h"
#include "../../math/color.h"

namespace Engine {

namespace SkyBox {

class Interface
{
    public:

    virtual ~Interface() = default;

    virtual Color getColor(const Vec3f& dir) const = 0;
};

} // namespace SkyBox

} // namespace Engine
