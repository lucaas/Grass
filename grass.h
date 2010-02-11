#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "vmath.h"
#include <cstdlib>
#include <math.h>
#include "segment.h"

#define NUM_SEGMENTS 5

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
    Grass(float x, float z);
    void init(float x, float z);

    ~Grass();

    void draw();
    void calculate(Vector3f wind, double timestep);
};
