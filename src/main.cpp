#include "Cloth.h"

#include <GL/glut.h>

#include <chrono>
#include <thread>

void DisplayCallback()
{
    static utad::Cloth cloth;

    cloth.Simulate();

    // Look at the center of the cloth
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float center = (cloth.m_MeshSize - 1) * cloth.m_Dx / 2.0f;
    gluLookAt(center, center, center * 3, center, center, 0.0f, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.333f, 1.0f, center * 20);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cloth.Draw();

    glutSwapBuffers();
}

void SetLightSource()
{
    GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat position[] = {0.0f, 0.0f, 1.0f, 0.0f};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void SetMaterial()
{
    GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuse[] = {0.8f, 0.4f, 0.4f, 1.0f};
    GLfloat specular[] = {0.9f, 0.2f, 0.2f, 1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("UTAD-SIM01-Cloth");
    glutDisplayFunc(DisplayCallback);
    glutIdleFunc(DisplayCallback);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    SetLightSource();
    SetMaterial();

    glutMainLoop();

    return 0;
}