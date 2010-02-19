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

    // Sätt fysikaliska konstanter utifrån segmentets längd.

    length = theLength;
    mass = length * 0.01f;
    springConstantXY = length*0.01f + 0.00001f*rand()/((float)(RAND_MAX));
    inertia = (mass * length * length) / 3.0f;



    angleXY = parentAngleXY;
    angularVelocityXY = 0.0f;

    angleZX = parentAngleZX;


    position = Vector3f();
    position.x = parent.x + length * cos(DEG2RAD(angleXY)) * sin(DEG2RAD(angleZX));
    position.y = parent.y + length * sin(DEG2RAD(angleXY));
    position.z = parent.z + length * cos(DEG2RAD(angleXY)) * cos(DEG2RAD(angleZX));



}

void Segment::calculatePosition(float windAngle, float windMagnitude, const Vector3f &parent, float parentAngleXY, double timestep)
{

    // Calculating force vector
    Vector3f force = Vector3f(windMagnitude * cos(DEG2RAD(windAngle)), 0.0f, windMagnitude * sin(DEG2RAD(windAngle)));
    force *= sin(DEG2RAD(angleXY));
    force.y = -GRAVITY * mass;

    // Ta ut en en fin vektor
    Vector3f tangularXY;
    tangularXY.x =  -1 *    sin(DEG2RAD(angleXY)) * cos(DEG2RAD(windAngle));
    tangularXY.y =  -1 *    cos(DEG2RAD(angleXY));
    tangularXY.z =  -1 *    sin(DEG2RAD(angleXY)) * sin(DEG2RAD(windAngle));


    // kraften i tangentens riktning
    float tangularForceXY =  tangularXY.dotProduct(force);
    float tauXY = (length/2.0)*tangularForceXY - springConstantXY*(angleXY - parentAngleXY);

    // vinkelhastighetsökning, steg ett (utifrån nuvarande vinkel)
    float k1 = (1.0f/inertia)*tauXY*timestep;

    // Kraft och hastighet steg två (utifrån ny vinkel)
    tauXY = (length/2.0)*tangularForceXY - springConstantXY*((angleXY+((angularVelocityXY+k1)*timestep)) - parentAngleXY);
    float k2 = (1.0f/inertia)*tauXY*timestep;

    // Ny hastighet = medelvärdet av föränringarna i steg ett och två, minus lite friktion
    angularVelocityXY = angularVelocityXY + (k1 + k2)/2.0f;
    angularVelocityXY *= FRICTION;

    // updatera  vinkel inom max/min-gräns
    angleXY = angleXY + angularVelocityXY*timestep;
    angleXY = (angleXY > 90 + 110) ? 90 + 110 : angleXY;
    angleXY = (angleXY < 90 - 110) ? 90 - 110 : angleXY;

    // uppdatera rotation i planet
    angleZX += 0.1*(windAngle - angleZX);

    // updatera position för segmentet
    position.x = parent.x + length * cos(DEG2RAD(angleXY)) * sin(DEG2RAD(angleZX));
    position.y = parent.y + length * sin(DEG2RAD(angleXY));
    position.z = parent.z + length * cos(DEG2RAD(angleXY)) * cos(DEG2RAD(angleZX));
}


float Segment::getAngleXY()
{
    return angleXY;
}

float Segment::getAngleZX()
{
    return angleZX;
}


