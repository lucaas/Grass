

// Grass segment
#ifndef __SEGMENT__
#define __SEGMENT__

#include "vmath.h"

class Segment
{
private:
    Vector3f position;
    float inertia;
    float springConstantXY;
    float springConstantZX;
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
    void calculatePosition(float windAngle, float windMagnitude, const Vector3f &parent, float parentAngleXY, float parentAngleZX, double timestep);
    Vector3f getPosition();
    float getAngleXY();
    float getAngleZX();

    void drawLine();

};


#endif
