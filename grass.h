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
    Vector2f getBase();

    ~Grass();

    void draw();
    void calculate(float windAngle,float windMagnitude, double timestep);
    Vector3f getPosition() const { return segments[0].getPosition(); }

};
