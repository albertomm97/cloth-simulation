#include "Cloth.h"

#include <GL/glut.h>

#include <iostream>

#define STRUCTURAL_SPRING_STIFFNESS 100.0f
#define SHEAR_SPRING_STIFFNESS 70.0f
#define FLEXION_SPRING_STIFFNESS 20.0f

#define ACCELERATION_OF_GRAVITY Vector3f(0.0f, -9.8f, 0.0f)
#define DAMPING_COEFFICIENT 0.1f
#define FLUID_VELOCITY Vector3f(0.0f, -30.0f, -50.0f)
#define FLUID_DAMPING_COEFFICIENT 1.0f

namespace utad
{
Cloth::Cloth()
{
    // Build particles
    for (int i = 0; i < m_MeshSize; i++) {
        std::vector<Particle> line;
        Vector3f origin(0.0f, m_Dx * i, 0.0f);
        for (int j = 0; j < m_MeshSize; j++) {
            Vector3f normal(0.0f, 0.0f, 1.0f);
            line.push_back(Particle(m_Mass, origin));
            origin += Vector3f(m_Dx, 0.0f, 0.0f);
        }
        m_Particles.push_back(line);
    }

    // Set two hanging points
    m_Particles[m_MeshSize - 1][0].m_Mass = 1e8f;
    m_Particles[m_MeshSize - 1][m_MeshSize - 1].m_Mass = 1e8f;
    //m_Particles[(m_MeshSize - 1) / 2][(m_MeshSize - 1) / 2].m_Mass = 1e8f;

    float structuralSpringRestLength = m_Dx;
    float shearSpringRestLength = structuralSpringRestLength * sqrtf(2.0f);
    float flexionSpringRestLength = structuralSpringRestLength * 2;
    float stiffness = 0.5f;

    auto size = m_Particles.size();
    for (int i = 0; i <= size-1; i++) {
        for (int j = 0; j <= size-1; j++) {

            // Comprobar esquinas
            if (i == 0 && j == 0) {

                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i + 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j + 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
            }

            if (i == 0 && j == size - 1) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i + 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j - 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
            }

            if (i == size - 1 && j == 0) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i - 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j + 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
            }

            if (i == size - 1 && j == size - 1) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i - 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j - 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                m_FlexionSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
            }

            // Fila de arriba
            if (i == 0 && (j > 0 && j < size - 1)) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i + 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j + 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j - 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));
                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                if (j > 1 && j < size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                    
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (j == 1){
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (j == size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                }
            }

            // Fila de abajo
            if (i == size-1 && (j > 0 && j < size - 1)) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i - 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j + 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j - 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));
                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                if (j > 1 && j < size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (j == 1) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (j == size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                }
            }

            // Fila izquierda
            if (j == 0 && (i > 0 && i < size - 1)) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i - 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j + 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i + 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));
                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                if (i > 1 && i < size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (i == 1) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (i == size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                }
            }

            // Fila derecha
            if (j == size-1 && (i > 0 && i < size - 1)) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i - 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j - 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i + 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));
                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                if (i > 1 && i < size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (i == 1) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                } else if (i == size - 2) {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                }
            }

            // Todos los demas  (gestionando la matriz interna sin contar los bordes)
            if ((i > 0 && i < size - 1) && (j > 0 && j < size - 1)) {
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i - 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j - 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i + 1][j],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));
                m_StructuralSprings.push_back(Spring(&m_Particles[i][j],
                                                     &m_Particles[i][j + 1],
                                                     structuralSpringRestLength,
                                                     STRUCTURAL_SPRING_STIFFNESS));

                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));
                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i + 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));
                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j - 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));
                m_ShearSprings.push_back(Spring(
                    &m_Particles[i][j], &m_Particles[i - 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

                // Fila izquierda
                if ((i > 0 && i < size-1) && j == 1) {
                    if (i == 1) {
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j + 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i + 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    }
                    else if (i == size-2) {
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j + 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i - 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    } else {
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j + 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i + 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i - 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    }
                }
                // Fila derecha
                else if ((i > 0 && i < size - 1) && j == size-2) {
                    if (i == 1) {
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j - 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i + 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    } else if (i == size - 2) {
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j - 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i - 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    } else {
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j - 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i + 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i - 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    }
                }

                // Fila arriba
                else if ((j > 0 && j < size - 1) && i == 1) {
                    if (!(j == 1) && !(j == size - 2)) {
                      
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j - 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j + 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i + 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    }
                }

                // Fila abajo
                else if ((j > 0 && j < size - 1) && i == size - 2) {
                    if (!(j == 1) && !(j == size - 2)) {
                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j - 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i][j + 2],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));

                        m_FlexionSprings.push_back(Spring(&m_Particles[i][j],
                                                          &m_Particles[i - 2][j],
                                                          flexionSpringRestLength,
                                                          FLEXION_SPRING_STIFFNESS));
                    }
                }
                else {
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j - 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));

                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i - 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                    m_FlexionSprings.push_back(Spring(
                        &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
                }
            }

            /*
            m_StructuralSprings.push_back(Spring(
                &m_Particles[i][j], &m_Particles[i + 1][j], structuralSpringRestLength, STRUCTURAL_SPRING_STIFFNESS));
            m_StructuralSprings.push_back(Spring(
                &m_Particles[i][j], &m_Particles[i][j + 1], structuralSpringRestLength, STRUCTURAL_SPRING_STIFFNESS));

             m_ShearSprings.push_back(
                Spring(&m_Particles[i][j], &m_Particles[i + 1][j + 1], shearSpringRestLength, SHEAR_SPRING_STIFFNESS));

             if (i < size - 2) {
                 m_FlexionSprings.push_back(Spring(
                     &m_Particles[i][j], &m_Particles[i + 2][j], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
             }
            
             if (j < size - 2) {
                 m_FlexionSprings.push_back(Spring(
                     &m_Particles[i][j], &m_Particles[i][j + 2], flexionSpringRestLength, FLEXION_SPRING_STIFFNESS));
             }
             */
        }
    }
}

void Cloth::Draw()
{
    // Draw springs
    glDisable(GL_LIGHTING);

    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < m_StructuralSprings.size(); i++)
        m_StructuralSprings[i].Draw();
    
    glColor3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < m_ShearSprings.size(); i++)
         m_ShearSprings[i].Draw();

    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < m_FlexionSprings.size(); i++)
        m_FlexionSprings[i].Draw();

    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < m_MeshSize; i++)
        for (int j = 0; j < m_MeshSize; j++)
            m_Particles[i][j].Draw();

    // Draw cloth mesh
    glEnable(GL_LIGHTING);

    for (int i = 0; i < m_MeshSize - 1; i++) {
        for (int j = 0; j < m_MeshSize - 1; j++) {
            const Particle &p0 = m_Particles[i][j];
            const Particle &p1 = m_Particles[i + 1][j];
            const Particle &p2 = m_Particles[i][j + 1];
            const Particle &p3 = m_Particles[i + 1][j + 1];

            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(p0.m_Normal.x, p0.m_Normal.y, p0.m_Normal.z);
            glVertex3f(p0.m_Position.x, p0.m_Position.y, p0.m_Position.z);

            glNormal3f(p1.m_Normal.x, p1.m_Normal.y, p1.m_Normal.z);
            glVertex3f(p1.m_Position.x, p1.m_Position.y, p1.m_Position.z);

            glNormal3f(p2.m_Normal.x, p2.m_Normal.y, p2.m_Normal.z);
            glVertex3f(p2.m_Position.x, p2.m_Position.y, p2.m_Position.z);

            glNormal3f(p3.m_Normal.x, p3.m_Normal.y, p3.m_Normal.z);
            glVertex3f(p3.m_Position.x, p3.m_Position.y, p3.m_Position.z);
            glEnd();
        }
    }
}

void Cloth::Simulate()
{
    // Compute the normal vector of each particle
    ComputeNormal();

    // Clear the force for all particles
    ClearForces();

    // Compute spring (internal) forces applied on each particle by Hooke's Law
    ComputeInternalForces();

    // Compute external forces applied on each particle
    ComputeExternalForces();

    // Compute the new cloth state by Euler's Method
    EulerIntegration();
}

void Cloth::ClearForces()
{
    // TODO
    for (auto& vparticle : m_Particles) 
    {
        for (auto& particle : vparticle) 
        {
            particle.ClearForces();
        }
    }
}

void Cloth::ComputeInternalForces()
{
    // Comprobar que esta forma de add la fuerza es la correcta
    for (auto& spring : m_StructuralSprings) {
        auto& force = spring.ComputeForce(); 
        spring.m_Particle1->m_Force += force;
        spring.m_Particle2->m_Force -= force;
    }

    for (auto &spring : m_ShearSprings) {
        auto &force = spring.ComputeForce();
        spring.m_Particle1->m_Force += force;
        spring.m_Particle2->m_Force -= force;
    }

    for (auto &spring : m_FlexionSprings) {
        auto &force = spring.ComputeForce();
        spring.m_Particle1->m_Force += force;
        spring.m_Particle2->m_Force -= force;
    }
 }

void Cloth::ComputeExternalForces()
{
    static float timer = 0.0f;
    timer += m_TimeStep;

    // Gravity
    Vector3f weight = ACCELERATION_OF_GRAVITY * m_Mass;

    for (int i = 0; i < m_MeshSize; i++) {
        for (int j = 0; j < m_MeshSize; j++) {
            // Damping
            Vector3f viscousDamping = -m_Particles[i][j].m_Velocity * DAMPING_COEFFICIENT;

            // Let the wind blow
            Vector3f windBlowForce =
                m_Particles[i][j].m_Normal *
                (m_Particles[i][j].m_Normal.dot(FLUID_VELOCITY * std::sin(timer) - m_Particles[i][j].m_Velocity)) *
                FLUID_DAMPING_COEFFICIENT;

            m_Particles[i][j].m_Force += weight + viscousDamping + windBlowForce;
        }
    }
}

void Cloth::EulerIntegration()
{
    auto size = m_Particles.size();
    int n = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            auto &p = m_Particles[i][j];
            auto acceleration = p.m_Force / p.m_Mass;
            p.m_Velocity = p.m_Velocity + (acceleration * m_TimeStep);
            p.m_Position = p.m_Position + (p.m_Velocity * m_TimeStep);
        }
    }
}

void Cloth::ComputeNormal()
{
    // Initialize the normal for all particles
    for (int i = 0; i < m_MeshSize; i++) {
        for (int j = 0; j < m_MeshSize; j++)
            m_Particles[i][j].m_Normal = Vector3f::ZERO;
    }

    // Compute the normal of each particle
    for (int i = 0; i < m_MeshSize - 1; i++) {
        for (int j = 0; j < m_MeshSize - 1; j++) {
            Vector3f v1;
            Vector3f v2;
            Vector3f normal;

            // Compute the normal of the upper triangle defined by the position of three particles
            v1 = m_Particles[i + 1][j + 1].m_Position - m_Particles[i][j].m_Position;
            v2 = m_Particles[i + 1][j].m_Position - m_Particles[i][j].m_Position;
            normal = v1.cross(v2).normalized();
            // Accumulate the normal
            m_Particles[i][j].m_Normal += normal;
            m_Particles[i + 1][j].m_Normal += normal;
            m_Particles[i + 1][j + 1].m_Normal += normal;

            // Compute the normal of the lower triangle defined by the position of another three particles
            v1 = m_Particles[i][j + 1].m_Position - m_Particles[i][j].m_Position;
            v2 = m_Particles[i + 1][j + 1].m_Position - m_Particles[i][j].m_Position;
            normal = v1.cross(v2).normalized();
            // Accumulate the normal
            m_Particles[i][j].m_Normal += normal;
            m_Particles[i][j + 1].m_Normal += normal;
            m_Particles[i + 1][j + 1].m_Normal += normal;
        }
    }

    // Normalize the normal for all particles
    for (int i = 0; i < m_MeshSize; i++) {
        for (int j = 0; j < m_MeshSize; j++)
            m_Particles[i][j].m_Normal.normalize();
    }
}
}  // namespace utad
