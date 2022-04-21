#include "Particle.h"

#include <GL/glut.h>

namespace utad
{
Particle::Particle(float mass, Vector3f position)
    : m_Mass(mass)
{
    m_Position = position;
    m_Velocity = Vector3f::ZERO;
    m_Force = Vector3f::ZERO;

    m_Normal = Vector3f::ZERO;
}

void Particle::ClearForces()
{
    m_Force = Vector3f::ZERO;
}

void Particle::Draw()
{
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f(m_Position.x, m_Position.y, m_Position.z);
    glEnd();
}
}  // namespace utad