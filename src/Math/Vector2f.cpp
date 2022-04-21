#include "Vector2f.h"

namespace utad
{
const Vector2f Vector2f::ZERO(0, 0);

Vector2f operator*(const float &k, const Vector2f &v)
{
    return Vector2f(k * v.x, k * v.y);
}

const Matrix2f Matrix2f::ZERO(0, 0, 0, 0);

const Matrix2f Matrix2f::IDENTITY(1, 0, 0, 1);

Matrix2f operator*(const float &k, const Matrix2f &m)
{
    return Matrix2f(m.v[0][0] * k, m.v[0][1] * k, m.v[1][0] * k, m.v[1][1] * k);
}
}  // namespace utad
