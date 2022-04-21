#ifndef VECTOR_3_F_H
#define VECTOR_3_F_H

#include <Std/CMath.h>

namespace utad
{
class Vector3f
{
public:
    float x, y, z;

public:
    Vector3f()
        : x(0)
        , y(0)
        , z(0)
    {
    }

    Vector3f(const float x, const float y, const float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    Vector3f operator-() const
    {
        Vector3f result(-x, -y, -z);

        return result;
    }

    void operator*=(const float k)
    {
        x *= k;
        y *= k;
        z *= k;
    }

    void operator+=(const Vector3f &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    void operator-=(const Vector3f &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    void operator*=(const Vector3f &v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    Vector3f operator+(const float &k) const { return Vector3f(x + k, y + k, z + k); }

    Vector3f operator-(const float &k) const { return Vector3f(x - k, y - k, z - k); }

    Vector3f operator*(const float &k) const { return Vector3f(x * k, y * k, z * k); }

    Vector3f operator/(const float &b) const
    {
        Vector3f result(x / b, y / b, z / b);

        return result;
    }

    Vector3f operator+(const Vector3f &v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }

    Vector3f operator-(const Vector3f &v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }

    Vector3f operator*(const Vector3f &v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }

    Vector3f operator/(const Vector3f &v) const { return Vector3f(x / v.x, y / v.y, z / v.z); }

    float dot(const Vector3f &v) const { return x * v.x + y * v.y + z * v.z; }

    Vector3f cross(const Vector3f &b) const
    {
        Vector3f result(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);

        return result;
    }

    float lengthSqr() const { return x * x + y * y + z * z; }

    float length() const { return std::sqrt(lengthSqr()); }

    void normalize()
    {
        float l = length();
        x = x / l;
        y = y / l;
        z = z / l;
    }

    Vector3f normalized() const
    {
        float l = length();
        Vector3f result(x / l, y / l, z / l);

        return result;
    }

public:
    const static Vector3f ZERO;
};
}  // namespace utad

#endif
