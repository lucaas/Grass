
#include "segment.h"

#define FRICTION 0.999
#define GRAVITY 9.82


Segment::Segment()
{
    Segment(Vector3f(0.0f,0.0f,0.0f), 90.0f);
}


Segment::Segment(const Vector3f &parent, float parentAngle)
{
    length = 0.3f;
    mass = length * 0.007f;
    springConstant = 0.5f;

    // Inertia = (0.003g * 0.5m * 0.5m) / 3
    inertia = (mass * length * length) / 3.0f;

    angle = parentAngle;

    position = Vector3f(parent.x + cos(angle)*length, parent.y + sin(angle)*length, parent.z);


}

void Segment::calculatePosition(const Vector3f &wind, const Vector3f &parent, float parentAngle, float timestep)
{


    Vector3f force = (wind + Vector3f(0.0, -GRAVITY*mass, 0.0));

    // acos() takes values in the interval [-1, 1], make sure we are in that range.

    float cosValue = position.dotProduct(force)/(position.length()*force.length());
    if (cosValue < -1) cosValue = -1;
    if (cosValue >  1) cosValue =  1;

    // TODO: we   need the direction of the force...
    // current direction only works if the grass is oriented in the x-direction
    float direction = (force.x > 0) ? 1 : -1;

    // sin(acos(x)) = cos(pi/2 - acos(x)) = sqrt(1-x*x)
    float tangularForce = direction * force.length() * sqrt(1 - cosValue*cosValue);

    float tau = (length/2.0)*tangularForce - springConstant*(angle - parentAngle);
    angularVelocity = angularVelocity + (1.0f/inertia)*tau*timestep;
    angularVelocity *= FRICTION;

    angle = angle + angularVelocity*timestep;

    position.x = parent.x + length * cos(DEG2RAD( parentAngle + angle));
    position.y = parent.y + length * sin(DEG2RAD( parentAngle + angle));
    position.z = parent.z;
}

Vector3f Segment::getPosition()
{
    return position;
}


float Segment::getAngle()
{
    return angle;
}

