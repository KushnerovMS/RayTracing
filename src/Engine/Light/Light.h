#pragma once

namespace Engine {

struct Light final
{
    Vec3f position;
    Color color = Color(1, 1, 1);

    Light(const Vec3f& pos = Vec3f(0, 0, 0),
          const Color& col = Color(1, 1, 1)) :
        position(pos), color(col) {}
};

} // namespace Engine
