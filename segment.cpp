#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "segment.h"
#include <cstdlib>

#define FRICTION 0.98
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

    springConstantXY = length*0.01f + 0.00001f*rand()/((float)(RAND_MAX));
    springConstantZX = length*0.01f;

  //  springConstantXY = 0.002f + 0.0001f*rand()/((float)(RAND_MAX));


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

void Segment::calculatePosition(float windAngle, float windMagnitude, const Vector3f &parent, float parentAngleXY, float parentAngleZX, double timestep)
{

    // sin(angleXY) makes it hard to move vertically when grass is close to the ground...
    // but also in the horisontal direction, which we dont want..
    Vector3f wind = Vector3f(windMagnitude * cos(DEG2RAD(windAngle)), 0.0f, windMagnitude * sin(DEG2RAD(windAngle)));

    Vector3f force = wind * sin(DEG2RAD(angleXY));
   // Vector3f force = wind;
    //Vector3f force = wind;
    force.y += -GRAVITY * mass;

    debugLine3 = position + force;


    Vector3f tangularXY;
    tangularXY.x =  -1 *    sin(DEG2RAD(angleXY)) * sin(DEG2RAD(windAngle));
     tangularXY.y = -1 * cos(DEG2RAD(angleXY));
   tangularXY.z =  -1 *    sin(DEG2RAD(angleXY)) * cos(DEG2RAD(windAngle));


    float tangularForceXY =  tangularXY.dotProduct(force);

    float tauXY = (length/2.0)*tangularForceXY - springConstantXY*(angleXY - parentAngleXY);
    angularVelocityXY = angularVelocityXY + (1.0f/inertia)*tauXY*timestep;
    angularVelocityXY *= FRICTION;
    angleXY = angleXY + angularVelocityXY*timestep;

    angleZX += 0.1*(windAngle - angleZX);

    float angle = 90.0f;

    position.x = parent.x + length * cos(DEG2RAD(angleXY)) * sin(DEG2RAD(angleZX));
    position.y = parent.y + length * sin(DEG2RAD(angleXY));
    position.z = parent.z + length * cos(DEG2RAD(angleXY)) * cos(DEG2RAD(angleZX));



  //  tangularXY *= tangularForceXY;
  //  tangularZX *= tangularForceZX;
    debugLine = position + tangularXY;

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

