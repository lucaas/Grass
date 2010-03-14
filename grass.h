#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "vmath.h"
#include <cstdlib>
#include <math.h>
#include "segment.h"

#ifndef __GRASS_H__
#define __GRASS_H__

class Grass
{
private:
    Vector3f base;
    Segment *segments;

    float initialAngleXY; // Jämviktsläge
    float initialAngleZX; // Jämviktsläge
    float colorShade;

public:
    Vector2f areabase;
    static int NUM_SEGMENTS;
    static float BASE_WIDTH;
    Grass();
    Grass(float x, float y, float z, float camX, float camZ);
    void init(float x, float y, float z);

    ~Grass();

    void draw();
    void calculate(float windAngle,float windMagnitude, double timestep);
    Vector3f getPosition() const { return segments[0].getPosition(); }
    Vector2f getBase() const { return Vector2f(base.x, base.z); };
    Vector2f getAreaBase() const { return Vector2f(areabase.x + base.x, areabase.y + base.z); };
};

#endif
