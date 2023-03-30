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
#include "../src/Engine/SkyBox/GradientSky.h"
#include "../src/Engine/Object/Object.h"
#include "../src/Engine/Light/Light.h"
#include "../src/Engine/Shape/Ellipsoid.h"

int main() {

    std::filesystem::remove_all("out");
    std::filesystem::create_directory("out");
    std::filesystem::remove("vid.gif");

    Engine::Scene scene;

    Engine::Camera camera;
    camera.setResolution(786, 786);
    camera.center = Vec3f(-5, 0, 0);
    camera.setDirection(Vec3f(1, 0, 0));
    scene.camera_p = &camera;

    Engine::SkyBox::Gradient skyBox (Color(0.5, 1, 0.5), Color(0.5, 0.5, 1));
    scene.skyBox_p = &skyBox;

    Engine::Light light1 (Vec3f(-5, 2, 2));
    scene.addLight_p(&light1);

    Engine::Light light2 (Vec3f(-5, 0, -2));
    scene.addLight_p(&light2);

    const int num = 5;
    Engine::Object ellps[num];
    Vec3f dir[num];
    float delay[num] = {};
    float timer[num] = {};
    std::srand(std::time(nullptr));
    for (int i = 0; i < num; i ++)
    {
        ellps[i].shape_p = Engine::Object::ShapePtr(Engine::Shape::Ellipsoid(Vec3f(0, 0, 0), 1, 1, 1));
        ellps[i].color = Color(0.5, 1.0, 0.5);
        ellps[i].albedo[1] = 0.3;
        scene.addObject_p(&ellps[i]);

        float phi = M_PI * float(std::rand()) / RAND_MAX;
        float theta = M_PI * float(std::rand()) / RAND_MAX;
        dir[i] = Vec3f(std::cos(phi) * std::cos(theta),
                       std::sin(theta),
                       std::sin(phi) * std::cos(theta));
        delay[i] = float(std::rand()) / RAND_MAX + 1.0f;
        timer[i] = float(std::rand()) / RAND_MAX;
    }

    const float dt = 1.0 / 25;
    const int frames = 1000;

    for (int i = 0; i < frames; i ++)
    {  
        for (int j = 0; j < num; j ++)
        {
            if (timer[j] < delay[j])
            {
                ellps[j].shape_p -> Scale(dir[j] * float(1 + 2.0 * std::sin(timer[j] / delay[j] * M_PI)));
                timer[j] += dt;
            }
            else
            {
                float phi = M_PI * float(std::rand()) / RAND_MAX;
                float theta = M_PI * float(std::rand()) / RAND_MAX;
                dir[j] = Vec3f(std::cos(phi) * std::cos(theta),
                               std::sin(theta),
                               std::sin(phi) * std::cos(theta));
                delay[j] = float(std::rand()) / RAND_MAX + 1.0f;
                timer[j] = 0;
            }
        }

        scene.Render(std::string("out/frame") + std::to_string(i) + ".ppm");

        std::cout << "\b\b\b" << i * 100 / frames << '%';
        std::cout.flush();
    }
    std::cout << std::endl;

    std::system("ffmpeg -r 25 -f image2 -s 786x786 -start_number 0 -i ./out/frame%01d.ppm -filter_complex '[0:v] fps=25,split [a][b];[a] palettegen [p];[b][p] paletteuse' vid.gif");


    return 0;
}
