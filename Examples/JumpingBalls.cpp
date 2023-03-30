#include <algorithm>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cfloat>
#include <filesystem>
#include <thread>

#include "../src/Engine/Camera/Camera.h"
#include "../src/Engine/Scene/Scene.h"
#include "../src/Engine/Shape/Ellipsoid.h"
#include "../src/Engine/SkyBox/GradientSky.h"
#include "../src/Engine/Object/Object.h"
#include "../src/Engine/Light/Light.h"

int main() {
    std::filesystem::remove_all("out");
    std::filesystem::create_directory("out");
    std::filesystem::remove("vid.gif");

    Engine::Scene scene;

    Engine::Camera camera;
    camera.setResolution(768, 768);
    camera.setDirection(Vec3f(1, 1, 0));
    scene.camera_p = &camera;

    Engine::SkyBox::Gradient skyBox (Color(0.5, 1, 0.5), Color(0.5, 0.5, 1));
    scene.skyBox_p = &skyBox;

    Engine::Light light1 (Vec3f(-5, 10, -5));
    scene.addLight_p(&light1);

    Engine::Light light2 (Vec3f(5, 2, 5));
    scene.addLight_p(&light2);

    Engine::Light light3 (Vec3f(0, 7, 0));
    scene.addLight_p(&light3);

    float r = 1;

    Engine::Object ellp1;
    ellp1.shape_p = Engine::Object::ShapePtr(Engine::Shape::Ellipsoid(Vec3f(0, 4, 2), r, r, r));
    ellp1.color = Color(1, 1, 0);
    ellp1.albedo[1] = 0.5;
    scene.addObject_p(&ellp1);

    Engine::Object ellp2;
    ellp2.shape_p = Engine::Object::ShapePtr(Engine::Shape::Ellipsoid(Vec3f(0, 2, -2), r, r, r));
    ellp1.color = Color(1, 0.5, 0.5);
    ellp1.albedo[1] = 0.7;
    scene.addObject_p(&ellp2);

    const float dt = 1.0 / 25;
    const Vec3f g = Vec3f(0, -9.8f, 0);
    Vec3f v1(0, 0, 0);
    Vec3f v2(0, 0, 0);

    float va_cam = 0.5;
    float vy_cam = 0.1;

    float k = 500;

    const int frames = 500;

    std::vector<Color> bufs[2];

    for (int i = 0; i < frames + 1; i ++)
    {
        camera.setDirection(camera.getDirection() + va_cam * dt * cross(camera.getDirection(), Vec3f(0, 1, 0)).normalize() - dt * Vec3f(0, vy_cam, 0));
        camera.center = Vec3f(0, 2, 0) - camera.getDirection() * 6.0f;

        float dy = ellp1.shape_p -> getCenter().y - r;
        if (dy <= 0)
        {
            v1 -= Vec3f(0, k, 0) * dt * (dy / r);
            ellp1.shape_p -> Scale(Vec3f(0, 1 + dy / r, 0));
        }
        else 
            ellp1.shape_p -> Scale(Vec3f(0, 1, 0));
        v1 += dt * g;
        ellp1.shape_p -> rMove(v1 * dt);

        dy = ellp2.shape_p -> getCenter().y - r;
        if (dy <= 0)
        {
            v2 -= Vec3f(0, k, 0) * dt * (dy / r);
            ellp2.shape_p -> Scale(Vec3f(0, 1 + dy / r, 0));
        }
        else 
            ellp2.shape_p -> Scale(Vec3f(0, 1, 0));
        v2 += dt * g;
        ellp2.shape_p -> rMove(v2 * dt);

        scene.Render(std::string("out/frame") + std::to_string(i) + ".ppm");

        std::cout << "\b\b\b" << i * 100 / frames << '%';
        std::cout.flush();
    }
    std::cout << std::endl;

    std::system("ffmpeg -r 25 -f image2 -s 786x786 -start_number 0 -i ./out/frame%01d.ppm -filter_complex '[0:v] fps=25,split [a][b];[a] palettegen [p];[b][p] paletteuse' vid.gif");

    return 0;
}
