#ifndef VECTOR_2_F_H
#define VECTOR_2_F_H

#include <Std/CMath.h>

namespace utad
{
class Vector2f
{
public:
    float x, y;

public:
    Vector2f()
        : x(0.0f)
        , y(0.0f)
    {
    }

    Vector2f(const float x, const float y)
        : x(x)
        , y(y)
    {
    }

    void operator*=(const float k)
    {
        x *= k;
        y *= k;
    }

    void operator+=(const Vector2f &v)
    {
        x += v.x;
        y += v.y;
    }

    void operator-=(const Vector2f &v)
    {
        x -= v.x;
        y -= v.y;
    }

    void operator*=(const Vector2f &v)
    {
        x *= v.x;
        y *= v.y;
    }

    Vector2f operator+(const float &k) const { return Vector2f(x + k, y + k); }

    Vector2f operator-(const float &k) const { return Vector2f(x - k, y - k); }

    Vector2f operator*(const float &k) const { return Vector2f(x * k, y * k); }

    Vector2f operator+(const Vector2f &v) const { return Vector2f(x + v.x, y + v.y); }

    Vector2f operator-(const Vector2f &v) const { return Vector2f(x - v.x, y - v.y); }

    Vector2f operator*(const Vector2f &v) const { return Vector2f(x * v.x, y * v.y); }

    Vector2f operator/(const Vector2f &v) const { return Vector2f(x / v.x, y / v.y); }

    float dot(const Vector2f &v) const { return x * v.x + y * v.y; }

    float cross(const Vector2f &v) const { return x * v.y - y * v.x; }

    Vector2f rotate90() const { return Vector2f(-y, x); }

    float lengthSqr() const { return x * x + y * y; }

    float length() const { return std::sqrt(lengthSqr()); }

public:
    const static Vector2f ZERO;

    static Vector2f crossProd(const Vector2f &v) { return Vector2f(-v.y, v.x); }
};

Vector2f operator*(const float &k, const Vector2f &v);

class Matrix2f
{
public:
    float v[2][2];

    Matrix2f()
    {
        v[0][0] = 0.0f;
        v[0][1] = 0.0f;
        v[1][0] = 0.0f;
        v[1][1] = 0.0f;
    }
    Matrix2f(float a, float b, float c, float d)
    {
        v[0][0] = a;
        v[0][1] = b;
        v[1][0] = c;
        v[1][1] = d;
    }

    void operator+=(const Matrix2f &m)
    {
        v[0][0] += m.v[0][0];
        v[0][1] += m.v[0][1];
        v[1][0] += m.v[1][0];
        v[1][1] += m.v[1][1];
    }
    void operator-=(const Matrix2f &m)
    {
        v[0][0] -= m.v[0][0];
        v[0][1] -= m.v[0][1];
        v[1][0] -= m.v[1][0];
        v[1][1] -= m.v[1][1];
    }

    Matrix2f operator+(const Matrix2f &m) const
    {
        return Matrix2f(v[0][0] + m.v[0][0], v[0][1] + m.v[0][1], v[1][0] + m.v[1][0], v[1][1] + m.v[1][1]);
    }

    Matrix2f operator-(const Matrix2f &m) const
    {
        return Matrix2f(v[0][0] - m.v[0][0], v[0][1] - m.v[0][1], v[1][0] - m.v[1][0], v[1][1] - m.v[1][1]);
    }

    Vector2f operator*(const Vector2f &m) const
    {
        return Vector2f(v[0][0] * m.x + v[0][1] * m.y, v[1][0] * m.x + v[1][1] * m.y);
    }

    Matrix2f operator*(const float &k) const { return Matrix2f(v[0][0] * k, v[0][1] * k, v[1][0] * k, v[1][1] * k); }

    Matrix2f operator*(const Matrix2f &m) const
    {
        return Matrix2f(v[0][0] * m.v[0][0] + v[0][1] * m.v[1][0],
                        v[0][0] * m.v[0][1] + v[0][1] * m.v[1][1],
                        v[1][0] * m.v[0][0] + v[1][1] * m.v[1][0],
                        v[1][0] * m.v[0][1] + v[1][1] * m.v[1][1]);
    }

    const static Matrix2f ZERO;

    const static Matrix2f IDENTITY;

    friend Matrix2f operator*(float k, const Matrix2f &m)
    {
        return Matrix2f(k * m.v[0][0], k * m.v[0][1], k * m.v[1][0], k * m.v[1][1]);
    }
};

Matrix2f operator*(const float &k, const Matrix2f &m);
}  // namespace utad

#endif
