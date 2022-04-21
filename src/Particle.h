#ifndef PARTICLE_H
#define PARTICLE_H

#include <Math/Vector3f.h>

namespace utad
{
class Particle
{
public:
    Particle(float mass, Vector3f position);

    void Draw();

    void ClearForces();

    float m_Mass;
    Vector3f m_Position;
    Vector3f m_Velocity;
    Vector3f m_Force;

    Vector3f m_Normal;
};
}  // namespace utad

#endif