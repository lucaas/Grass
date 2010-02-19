

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

    float length;
    float mass;

    float angleXY;
    float angularVelocityXY;

    float angleZX;



public:
    Segment();
    Segment(const Vector3f &parent, float parentAngleXY, float parentAngleZX, float theLength = 0.3f);
    void init(const Vector3f &parent, float parentAngleXY, float parentAngleZX, float theLength);
    void calculatePosition(float windAngle, float windMagnitude, const Vector3f &parent, float parentAngleXY, double timestep);
    Vector3f getPosition() const { return position; }
    float getAngleXY();
    float getAngleZX();

};


#endif
