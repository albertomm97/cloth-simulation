#ifndef CLOTH_H
#define CLOTH_H

#include <Spring.h>

#include <vector>

namespace utad
{
class Cloth
{
public:
    Cloth();

    void Draw();
    void Simulate();

    void ClearForces();
    void ComputeInternalForces();
    void ComputeExternalForces();
    void EulerIntegration();

    void ComputeNormal();

    int m_MeshSize{25};

    float m_Mass{1.0F};
    float m_TimeStep{0.01F};
    float m_Dx{5.0F};

    std::vector<std::vector<Particle> > m_Particles;
    std::vector<Spring> m_StructuralSprings;
    std::vector<Spring> m_ShearSprings;
    std::vector<Spring> m_FlexionSprings;
};
}  // namespace utad

#endif