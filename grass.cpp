#include "grass.h"
#include <cstdlib>

Grass::Grass()
{

    // Random nummer mellan -10 och 10
    float xpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;
    float zpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;

    base[0] = xpos;
    base[1] = 0.0f;
    base[2] = zpos;

}

Grass::~Grass()
{
    delete base;
}

void Grass::draw()
{
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-0.1f + base[0], 0.0f + base[1], 1.0f + base[2]);
        glVertex3f( 0.1f + base[0], 0.0f + base[1], 1.0f + base[2]);
        glVertex3f( 0.0f + base[0], 0.8f + base[1], 0.9f + base[2]);
        glVertex3f( 0.2f + base[0], 0.8f + base[1], 0.9f + base[2]);
        glVertex3f( 0.1f + base[0], 1.4f + base[1], 0.5f + base[2]);
        glVertex3f( 0.3f + base[0], 1.4f + base[1], 0.5f + base[2]);
        glVertex3f( 0.2f + base[0], 1.6f + base[1], 0.0f + base[2]);
        glVertex3f( 0.4f + base[0], 1.6f + base[1], 0.0f + base[2]);
    glEnd();
}
