#include <fstream>
#include <vector>

#include "Scene.h"
#include "../../math/geometry.h"
#include "../../math/color.h"

Vec3f reflect(const Vec3f& I, const Vec3f N)
{
    return I - 2.0f * N *(I * N);
}

Color Engine::Scene::CastRay_(const Vec3f &orig, const Vec3f &dir) const
{
    float dist = std::numeric_limits<float>::max();
    Color resColor = Color(0, 0, 0);
    const Object* intObj = nullptr;

    for (const Object* obj : objects_p)
        if (obj -> shape_p -> RayIntersect(orig, dir, dist))
            intObj = obj;

    if (intObj == nullptr)
        return skyBox_p -> getColor(dir);

    Vec3f intPos = orig + dist * dir;
    Vec3f n = intObj -> shape_p -> GetN(intPos);

    for (const Light* light : lights_p)
    {
        Vec3f lightDir = light -> position - intPos;
        float lightDist = lightDir.norm();
        lightDir.normalize();
        float k = n * lightDir;
        if (k <= 0) continue;

        bool intersected = false;
        for (const Object* obj : objects_p)
            if (obj != intObj && obj -> shape_p -> RayIntersect(intPos, lightDir, lightDist))
            {
                intersected = true;
                break;
            }

        if (!intersected)
        {
            resColor += (intObj -> albedo[0] * intObj -> color * light -> color * k +
                        intObj -> albedo[1] * light -> color *
                        std::pow(lightDir.normalize() * reflect(dir, n).normalize(), intObj -> specExp));
            //resColor *= 0.5f;
        }
    }

    return resColor;
}

void Engine::Scene::Render(std::vector<Color>* buf_p) const
{
    int bufferWidth = camera_p -> getWidth();
    int bufferHeight = camera_p -> getHeight();
    std::vector<Color>& framebuffer = *buf_p;
    framebuffer.resize (bufferWidth * bufferHeight);
    const int fov      = M_PI/2.;

#pragma omp parallel num_threads(6)
{
    for (size_t j = 0; j < bufferHeight; j ++) {
        for (size_t i = 0; i < bufferWidth; i++) {
            framebuffer[i + j * bufferWidth] =
                CastRay_(camera_p -> center, camera_p -> getRay(i, j));
            framebuffer[i + j * bufferWidth].normalize();
        }
    }
}
}

void Engine::WriteBuf(const std::vector<Color>* buf_p, const int width, const int height, const std::string& fileName)
{
    const std::vector<Color>& framebuffer = *buf_p;

    std::ofstream ofs; // save the framebuffer to file
    ofs.open(fileName);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < width * height; ++i) {
        for (size_t j = 0; j<3; j++)
            ofs << (char)(255 * framebuffer[i][j]);
    }
    ofs.close();
}

void Engine::Scene::Render(const std::string& fileName) const
{
    std::vector<Color> buf;
    Render(&buf);
    WriteBuf(&buf, camera_p -> getWidth(), camera_p -> getHeight(), fileName);
}
