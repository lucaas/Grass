

// Grass segment
#ifndef __SEGMENT__
#define __SEGMENT__

#include "vmath.h"

class Segment
{
private:
    Vector3f position;
    float inertia;
    float springConstant;
    float length;
    float mass;

    float angleXY;
    float angularVelocityXY;

    float angleZX;
    float angularVelocityZX;

    Vector3f debugLine, debugLine2, debugLine3;


public:
    Segment();
    Segment(const Vector3f &parent, float parentAngleXY, float parentAngleZX, float theLength = 0.3f);
    void init(const Vector3f &parent, float parentAngleXY, float parentAngleZX, float theLength);
    void calculatePosition(const Vector3f &wind, const Vector3f &parent, float parentAngleXY, float parentAngleZX, double timestep);
    Vector3f getPosition();
    float getAngleXY();
    float getAngleZX();

    void drawLine();

};


#endif
