
#include "segment.h"

#define FRICTION 0.99
#define GRAVITY 9.82


Segment::Segment()
{
    init(Vector3f(0.0f,0.0f,0.0f), 90.0f, 0.3f);
}

Segment::Segment(const Vector3f &parent, float parentAngle, float theLength)
{
    init(parent,parentAngle, theLength);
}


void Segment::init(const Vector3f &parent, float parentAngle, float theLength)
{
    length = theLength;
    mass = length * 0.07f;
    springConstant = 0.02f + 0.01f*rand()/((float)(RAND_MAX));

    // Inertia = (0.003g * 0.5m * 0.5m) / 3
    inertia = (mass * length * length) / 3.0f;

    angle = parentAngle;
    angularVelocity = 0.0f;

    position = Vector3f(parent.x + cos(parentAngle)*length, parent.y + sin(parentAngle)*length, parent.z);


}

void Segment::calculatePosition(const Vector3f &wind, const Vector3f &parent, float parentAngle, double timestep)
{


    float windx = wind.x * sin(DEG2RAD(angle));
    Vector3f force = Vector3f(windx, -GRAVITY*mass, 0.0);

    // acos() takes values in the interval [-1, 1], make sure we are in that range.
    Vector3f parentToChild = position-parent;
    float cosValue = parentToChild.dotProduct(force)/(parentToChild.length()*force.length());
    if (cosValue < -1) cosValue = -1;
    if (cosValue >  1) cosValue =  1;

    // TODO: we   need the direction of the force...
    // current direction only works if the grass is oriented in the x-direction
    float direction = (force.x > 0) ? -1: 1;

    // sin(acos(x)) = cos(pi/2 - acos(x)) = sqrt(1-x*x)
    float tangularForce = direction * force.length() * sqrt(1 - cosValue*cosValue);

  //  printf("force : %f\t%f\t%f\n", force.length(), angle, parentAngle);

    float tau = (length/2.0)*tangularForce - springConstant*(angle - parentAngle);

    angularVelocity = angularVelocity + (1.0f/inertia)*tau*timestep;
    angularVelocity *= FRICTION;


    angle = angle + angularVelocity*timestep;



    position.x = parent.x + length * cos(DEG2RAD(angle));
    position.y = parent.y + length * sin(DEG2RAD(angle));
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

