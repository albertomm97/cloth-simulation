#ifndef SPRING_H
#define SPRING_H

#include "Particle.h"

namespace utad
{
class Spring
{
public:
    Spring(Particle *, Particle *, float, float);

    void Draw();
    float ComputeLength();
    Vector3f ComputeForce();

    Particle *m_Particle1;
    Particle *m_Particle2;
    const float m_RestLength;
    const float m_Stiffness;
};
}  // namespace utad

#endif