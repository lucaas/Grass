#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "vmath.h"
#include <cstdlib>
#include <math.h>
#include "segment.h"

#define NUM_SEGMENTS 3
#define BASE_WIDTH  0.15

#ifndef __GRASS_H__
#define __GRASS_H__

class Grass
{
private:
    Vector3f base;
    Segment segments[NUM_SEGMENTS];

    float initialAngleXY; // Jämviktsläge
    float initialAngleZX; // Jämviktsläge
    float colorShade;

public:
    Grass();
    Grass(float x, float y, float z);
    void init(float x, float y, float z);

    ~Grass();

    void draw();
    void calculate(float windAngle,float windMagnitude, double timestep);
    Vector3f getPosition() const { return segments[0].getPosition(); }
    Vector2f getBase() const { return Vector2f(base.x, base.z); };
};

#endif
