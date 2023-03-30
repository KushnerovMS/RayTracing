#include "Ellipsoid.h"
#include "Interface.h"
#include <algorithm>
#include <memory>

Engine::Shape::Ellipsoid::Ellipsoid(const Vec3f &center, float a, float b, float c) :
    center_(center),
    x2c0_(1.0f/a/a),    y2c0_(1.0f/b/b),    z2c0_(1.0f/c/c),
    x2c_(x2c0_),        y2c_(y2c0_),        z2c_(z2c0_),
    xyc0_(0),           yzc0_(0),           zxc0_(0),
    xyc_(0),            yzc_(0),            zxc_(0)
{}

std::unique_ptr<Engine::Shape::Interface> Engine::Shape::Ellipsoid::Clone() const
{
    return std::unique_ptr<Engine::Shape::Interface>(new Ellipsoid(*this));
}


bool Engine::Shape::Ellipsoid::RayIntersect
    (const Vec3f& orig, const Vec3f& dir, float& t0) const
{
        Vec3f L = orig - center_;

        float a1 = dir[0];
        float b1 = dir[1];
        float c1 = dir[2];

        float x0 = L[0];
        float y0 = L[1];
        float z0 = L[2];

        float A =   x2c_ * a1 * a1 +
                    y2c_ * b1 * b1 +
                    z2c_ * c1 * c1 +
                    
                    xyc_ * a1 * b1 +
                    yzc_ * b1 * c1 +
                    zxc_ * c1 * a1;

        float B =   2 * x2c_ * a1 * x0 +
                    2 * y2c_ * b1 * y0 +
                    2 * z2c_ * c1 * z0 +
                    
                    xyc_ * (a1 * y0 + b1 * x0) +
                    yzc_ * (b1 * z0 + c1 * y0) +
                    zxc_ * (c1 * x0 + a1 * z0);
        B /= 2.0f;

        float C =   x2c_ * x0 * x0 +
                    y2c_ * y0 * y0 +
                    z2c_ * z0 * z0 +

                    xyc_ * x0 * y0 +
                    yzc_ * y0 * z0 +
                    zxc_ * z0 * x0 +
                    
                    - 1;

        if (B * B > A * C)
        {
            float sqrt_D = std::sqrt(B * B - A * C);
            float t1 = (- B - sqrt_D) / A;
            float t2 = (- B + sqrt_D) / A;
            bool isIntersect = false;
            if (t1 > 0 && t1 < t0) { t0 = t1; isIntersect = true; }
            if (t2 > 0 && t2 < t0) { t0 = t2; isIntersect = true; }
                
            return isIntersect;
        }
        else
            return false;
}

Vec3f Engine::Shape::Ellipsoid::GetN(const Vec3f& pos) const
{
    Vec3f relPos = pos - center_;

    Vec3f v1 (-relPos.y * 2.0f * y2c_ - relPos.x * xyc_, relPos.x * 2.0f * x2c_ + relPos.y * xyc_, 0);
    Vec3f v2 (-relPos.z * 2.0f * z2c_ - relPos.x * zxc_, 0, relPos.x * 2.0f * x2c_ + relPos.z * zxc0_);

    Vec3f n = cross(v1, v2);

    return ((n * relPos) * n).normalize();//((n * relPos) * n).normalize();
}

void Engine::Shape::Ellipsoid::SaveState()
{
    x2c0_ = x2c_;
    y2c0_ = y2c_;
    z2c0_ = z2c_;

    xyc0_ = xyc_;
    yzc0_ = yzc_;
    zxc0_ = zxc_;
}


void Engine::Shape::Ellipsoid::Scale(const Vec3f& dir)
{
    Vec3f ndir = dir;
    ndir.normalize();
    float scale = dir.norm();

    Vec3f x = Vec3f(1, 0, 0);
    Vec3f rotDir = cross(x, ndir);
    float angle = std::asin(rotDir.norm());
    rotDir.normalize();
    x.rotate(angle, rotDir);
    Vec3f y = Vec3f(0, 1, 0).rotate(angle, rotDir);
    Vec3f z = Vec3f(0, 0, 1).rotate(angle, rotDir);

    x.x /= scale;
    y.x /= scale;
    z.x /= scale;

    x.rotate(-angle, rotDir);
    y.rotate(-angle, rotDir);
    z.rotate(-angle, rotDir);

    x2c_ =  x2c0_ * x.x * x.x +
            y2c0_ * x.y * x.y +
            z2c0_ * x.z * x.z +
            
            xyc0_ * x.x * x.y +
            yzc0_ * x.y * x.z +
            zxc0_ * x.z * x.x;

    y2c_ =  x2c0_ * y.x * y.x +
            y2c0_ * y.y * y.y +
            z2c0_ * y.z * y.z +
            
            xyc0_ * y.x * y.y +
            yzc0_ * y.y * y.z +
            zxc0_ * y.z * y.x;

    z2c_ =  x2c0_ * z.x * z.x +
            y2c0_ * z.y * z.y +
            z2c0_ * z.z * z.z +
            
            xyc0_ * z.x * z.y +
            yzc0_ * z.y * z.z +
            zxc0_ * z.z * z.x;


    xyc_ =  2.0f * x2c0_ * x.x * y.x +
            2.0f * y2c0_ * x.y * y.y +
            2.0f * z2c0_ * x.z * y.z +

            xyc0_ * (x.x * y.y + y.x * x.y) +
            yzc0_ * (x.y * y.z + y.y * x.z) +
            zxc0_ * (x.z * y.x + y.z * x.x);

    yzc_ =  2.0f * x2c0_ * y.x * z.x +
            2.0f * y2c0_ * y.y * z.y +
            2.0f * z2c0_ * y.z * z.z +

            xyc0_ * (y.x * z.y + z.x * y.y) +
            yzc0_ * (y.y * z.z + z.y * y.z) +
            zxc0_ * (y.z * z.x + z.z * y.x);

    zxc_ =  2.0f * x2c0_ * z.x * x.x +
            2.0f * y2c0_ * z.y * x.y +
            2.0f * z2c0_ * z.z * x.z +

            xyc0_ * (z.x * x.y + x.x * z.y) +
            yzc0_ * (z.y * x.z + x.y * z.z) +
            zxc0_ * (z.z * x.x + x.z * z.x);
}

void Engine::Shape::Ellipsoid::rScale(const Vec3f& dir)
{
    x2c_ *= dir.x * dir.x;
    y2c_ *= dir.y * dir.y;
    z2c_ *= dir.z * dir.z;
}


void Engine::Shape::Ellipsoid::Rotate(float angle, const Vec3f dir)
{
    Vec3f x = Vec3f(1, 0, 0).rotate(angle, dir);
    Vec3f y = Vec3f(0, 1, 0).rotate(angle, dir);
    Vec3f z = Vec3f(0, 0, 1).rotate(angle, dir);

    x2c_ =  x2c0_ * x.x * x.x +
            y2c0_ * x.y * x.y +
            z2c0_ * x.z * x.z +
            
            xyc0_ * x.x * x.y +
            yzc0_ * x.y * x.z +
            zxc0_ * x.z * x.x;

    y2c_ =  x2c0_ * y.x * y.x +
            y2c0_ * y.y * y.y +
            z2c0_ * y.z * y.z +
            
            xyc0_ * y.x * y.y +
            yzc0_ * y.y * y.z +
            zxc0_ * y.z * y.x;

    z2c_ =  x2c0_ * z.x * z.x +
            y2c0_ * z.y * z.y +
            z2c0_ * z.z * z.z +
            
            xyc0_ * z.x * z.y +
            yzc0_ * z.y * z.z +
            zxc0_ * z.z * z.x;


    xyc_ =  2.0f * x2c0_ * x.x * y.x +
            2.0f * y2c0_ * x.y * y.y +
            2.0f * z2c0_ * x.z * y.z +

            xyc0_ * (x.x * y.y + y.x * x.y) +
            yzc0_ * (x.y * y.z + y.y * x.z) +
            zxc0_ * (x.z * y.x + y.z * x.x);

    yzc_ =  2.0f * x2c0_ * y.x * z.x +
            2.0f * y2c0_ * y.y * z.y +
            2.0f * z2c0_ * y.z * z.z +

            xyc0_ * (y.x * z.y + z.x * y.y) +
            yzc0_ * (y.y * z.z + z.y * y.z) +
            zxc0_ * (y.z * z.x + z.z * y.x);

    zxc_ =  2.0f * x2c0_ * z.x * x.x +
            2.0f * y2c0_ * z.y * x.y +
            2.0f * z2c0_ * z.z * x.z +

            xyc0_ * (z.x * x.y + x.x * z.y) +
            yzc0_ * (z.y * x.z + x.y * z.z) +
            zxc0_ * (z.z * x.x + x.z * z.x);
}

void Engine::Shape::Ellipsoid::rRotate(float angle, const Vec3f dir)
{
    Vec3f x = Vec3f(1, 0, 0).rotate(angle, dir);
    Vec3f y = Vec3f(0, 1, 0).rotate(angle, dir);
    Vec3f z = Vec3f(0, 0, 1).rotate(angle, dir);

    x2c_ =  x2c_ * x.x * x.x +
            y2c_ * x.y * x.y +
            z2c_ * x.z * x.z +
            
            xyc_ * x.x * x.y +
            yzc_ * x.y * x.z +
            zxc_ * x.z * x.x;

    y2c_ =  x2c_ * y.x * y.x +
            y2c_ * y.y * y.y +
            z2c_ * y.z * y.z +
            
            xyc_ * y.x * y.y +
            yzc_ * y.y * y.z +
            zxc_ * y.z * y.x;

    z2c_ =  x2c_ * z.x * z.x +
            y2c_ * z.y * z.y +
            z2c_ * z.z * z.z +
            
            xyc_ * z.x * z.y +
            yzc_ * z.y * z.z +
            zxc_ * z.z * z.x;


    xyc_ =  2.0f * x2c_ * x.x * y.x +
            2.0f * y2c_ * x.y * y.y +
            2.0f * z2c_ * x.z * y.z +

            xyc_ * (x.x * y.y + y.x * x.y) +
            yzc_ * (x.y * y.z + y.y * x.z) +
            zxc_ * (x.z * y.x + y.z * x.x);

    yzc_ =  2.0f * x2c_ * y.x * z.x +
            2.0f * y2c_ * y.y * z.y +
            2.0f * z2c_ * y.z * z.z +

            xyc_ * (y.x * z.y + z.x * y.y) +
            yzc_ * (y.y * z.z + z.y * y.z) +
            zxc_ * (y.z * z.x + z.z * y.x);

    zxc_ =  2.0f * x2c_ * z.x * x.x +
            2.0f * y2c_ * z.y * x.y +
            2.0f * z2c_ * z.z * x.z +

            xyc_ * (z.x * x.y + x.x * z.y) +
            yzc_ * (z.y * x.z + x.y * z.z) +
            zxc_ * (z.z * x.x + x.z * z.x);
}
