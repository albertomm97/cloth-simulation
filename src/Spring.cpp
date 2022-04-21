#include "Spring.h"

#include <GL/glut.h>

namespace utad
{
Spring::Spring(Particle *p1, Particle *p2, float restLength, float stiffness)
    : m_RestLength(restLength)
    , m_Stiffness(stiffness)
{
    m_Particle1 = p1;
    m_Particle2 = p2;
}

void Spring::Draw()
{
    glBegin(GL_LINES);
    glVertex3f(m_Particle1->m_Position.x, m_Particle1->m_Position.y, m_Particle1->m_Position.z);
    glVertex3f(m_Particle2->m_Position.x, m_Particle2->m_Position.y, m_Particle2->m_Position.z);
    glEnd();
}

float Spring::ComputeLength()
{
    auto& p = m_Particle2->m_Position - m_Particle1->m_Position;
    return p.length();
}

Vector3f Spring::ComputeForce()
{
    auto &p1 = m_Particle1->m_Position;
    auto &p2 = m_Particle2->m_Position;

    auto &pf = p1 - p2;
    auto mod = pf.length();

    pf = pf / mod;

    return pf * (m_Stiffness * (m_RestLength - mod));
}

}  // namespace utad