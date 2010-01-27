#include "grass.h"
#include <cstdlib>
#include <math.h>

Grass::Grass()
{

    // Random nummer mellan -10 och 10
    float xpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;
    float zpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;

    base[0] = xpos;
    base[1] = 0.0f;
    base[2] = zpos;

    theta0 = 90 + 20.0*(rand()/float(RAND_MAX)) - 10.0;
    theta = theta0;

    radius1 = 1.8f;
    K = 1.337f;
    inertia = 1.0f;

}

Grass::Grass(float x, float z)
{
    base[0] = x;
    base[1] = 0.0f;
    base[2] = z;

    theta0 = 90 + 20.0*(rand()/float(RAND_MAX)) - 10.0;
    theta = theta0;

    radius1 = 1.8f;
    K = 1.337f;
    inertia = 1.0f;
}

Grass::~Grass()
{
    delete base;
}

void Grass::calculate()
{
}

void Grass::draw()
{
    glBegin(GL_LINES);
        glVertex3f(base[0], base[1], base[2]);
        glVertex3f(base[0] + radius1*cos(theta*(3.14159f/180.0f)), base[1] + radius1*sin(theta*(3.14159f/180.0f)), base[2]);
    glEnd();
}
