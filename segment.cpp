#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "segment.h"
#include <cstdlib>

#define FRICTION 0.97
#define GRAVITY 9.82


Segment::Segment()
{
    init(Vector3f(0.0f,0.0f,0.0f), 90.0f, 0.0f, 0.3f);
}

Segment::Segment(const Vector3f &parent, float parentAngleXY, float parentAngleZX, float theLength)
{
    init(parent,parentAngleXY, parentAngleZX, theLength);
}


void Segment::init(const Vector3f &parent, float parentAngleXY, float parentAngleZX, float theLength)
{
    length = theLength;
    mass = length * 0.01f;
    springConstant = 0.002f + 0.0001f*rand()/((float)(RAND_MAX));

    // Inertia = (0.003g * 0.5m * 0.5m) / 3
    inertia = (mass * length * length) / 3.0f;

    angleXY = parentAngleXY;
    angularVelocityXY = 0.0f;

    angleZX = parentAngleZX;
    angularVelocityZX = 0.0f;

    position = Vector3f();
    position.x = parent.x + length * cos(DEG2RAD(angleXY)) * sin(DEG2RAD(angleZX));
    position.y = parent.y + length * sin(DEG2RAD(angleXY));
    position.z = parent.z + length * cos(DEG2RAD(angleXY)) * cos(DEG2RAD(angleZX));



}

void Segment::calculatePosition(const Vector3f &wind, const Vector3f &parent, float parentAngleXY, float parentAngleZX, double timestep)
{

    // sin(angleXY) makes it hard to move vertically when grass is close to the ground...
    // but also in the horisontal direction, which we dont want..
    Vector3f force = wind * sin(DEG2RAD(angleXY));
    //Vector3f force = wind;
    force.y += -GRAVITY * mass;

    debugLine3 = position + force;

/*
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
*/



    // troligtvis fuck'd, beräkna själv ist för att copy-paste:a wolfram
    Vector3f tangularZX;
    tangularZX.x = cos(DEG2RAD(angleZX));
    tangularZX.y = 0.0f;
    tangularZX.z = -1 * sin(DEG2RAD(angleZX));




    float cosValueZX = tangularZX.dotProduct(force)/(force.length());
    if (cosValueZX < -1) cosValueZX = -1;
    if (cosValueZX >  1) cosValueZX =  1;
    // fel?
    float directionZX = (cosValueZX < 0) ? -1: 1;
    float tangularForceZX = directionZX * force.length() * sqrt(1 - cosValueZX*cosValueZX);



    Vector3f tangularXY;
    tangularXY.x = sin(DEG2RAD(angleZX)) * cos(DEG2RAD(angleXY));
    tangularXY.y = -1 * sin(DEG2RAD(angleXY));
    tangularXY.z = cos(DEG2RAD(angleZX)) * cos(DEG2RAD(angleXY));

    float cosValueXY = tangularXY.dotProduct(force)/(force.length());
    if (cosValueXY < -1) cosValueXY = -1;
    if (cosValueXY >  1) cosValueXY =  1;
    // fel?
    float directionXY= (cosValueXY < 0) ? -1: 1;
    float tangularForceXY = directionXY * force.length() * sqrt(1 - cosValueXY*cosValueXY);



    float tauXY = (length/2.0)*tangularForceXY - springConstant*(angleXY - parentAngleXY);
    angularVelocityXY = angularVelocityXY + (1.0f/inertia)*tauXY*timestep;
    angularVelocityXY *= FRICTION;
    angleXY = angleXY + angularVelocityXY*timestep;


    float tauZX = (length/2.0)*tangularForceZX - springConstant*(angleZX - parentAngleZX);

    angularVelocityZX = angularVelocityZX + (1.0f/inertia)*tauZX*timestep;
    angularVelocityZX *= FRICTION;
    angleZX = angleZX + angularVelocityZX*timestep;


    position.x = parent.x + length * cos(DEG2RAD(angleXY)) * sin(DEG2RAD(angleZX));
    position.y = parent.y + length * sin(DEG2RAD(angleXY));
    position.z = parent.z + length * cos(DEG2RAD(angleXY)) * cos(DEG2RAD(angleZX));

    tangularXY *= tangularForceXY;
    tangularZX *= tangularForceZX;
    debugLine = position + tangularXY;
    debugLine2 = position + tangularZX;

    printf("angles: %f\n", tauXY-tauZX);

}

Vector3f Segment::getPosition()
{
    return position;
}


float Segment::getAngleXY()
{
    return angleXY;
}

float Segment::getAngleZX()
{
    return angleZX;
}

void Segment::drawLine() {

    glLineWidth(3.0);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0, 1.0f);
        glVertex3f(position.x, position.y, position.z);
        glVertex3f(debugLine.x, debugLine.y, debugLine.z);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0, 0.0f);
        glVertex3f(position.x, position.y, position.z);
        glVertex3f(debugLine2.x, debugLine2.y, debugLine2.z);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0.0f, 1.0, 1.0f);
        glVertex3f(position.x, position.y, position.z);
        glVertex3f(debugLine3.x, debugLine3.y, debugLine3.z);
    glEnd();


}

