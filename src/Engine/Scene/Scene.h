#pragma once

#include <string>
#include <list>
#include <vector>

#include "../Camera/Camera.h"
#include "../SkyBox/SkyBox.h"
#include "../Object/Object.h"
#include "../Light/Light.h"

namespace Engine {

class Scene final
{
    std::list<const Object*> objects_p;
    std::list<const Light*>  lights_p;

    Color CastRay_(const Vec3f &orig, const Vec3f &dir) const;

    public:

    const Camera* camera_p = nullptr;
    const SkyBox::Interface* skyBox_p = nullptr;

    Scene() {}

    void Render(std::vector<Color>* buf) const;
    void Render(const std::string& fileName) const;

    void addObject_p(const Object* obj) { objects_p.push_back(obj); }
    void addLight_p(const Light* light) { lights_p.push_back(light); }
};

void WriteBuf(const std::vector<Color>* buf, const int width, const int height, const std::string& fileName);

} // namespace Engine
