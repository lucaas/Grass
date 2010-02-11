#include "grass.h"
#include "segment.h"

#define BASE_WIDTH  0.1

Grass::Grass()
{

    // Random nummer mellan -10 och 10
    float xpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;
    float zpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;

    init(xpos,zpos);
}

Grass::Grass(float x, float z)
{
    init(x,z);
}

void Grass::init(float x, float z)
{
    base.x = x;
    base.y = 0.0f;
    base.z = z;

    initialAngleXY = 80.0f + 20.0*(rand()/float(RAND_MAX)); // 80-100 degrees
    initialAngleZX = 360.0*(rand()/float(RAND_MAX)); // 0-360 degrees
    //initialAngleZX = 180.0f;

    // 0.75- 1.25 * (0.4 + 0.25 + 0.2)
	float lengthMultiplier = 1.75f + 0.5*(rand()/float(RAND_MAX)); // 0.75-1 * length

    segments[0] = Segment(base, initialAngleXY, initialAngleZX, lengthMultiplier * 0.4f);

    for (int i=1; i < NUM_SEGMENTS; i++)
        segments[i] = Segment(segments[i-1].getPosition(), segments[i-1].getAngleXY(), initialAngleZX, lengthMultiplier * (0.3f/(0.5f*(i+1))));


    colorShade = 0.2*rand()/float(RAND_MAX) + 0.65;


}

Grass::~Grass()
{
    delete segments;
}

void Grass::calculate(Vector3f wind, double timestep)
{
    segments[0].calculatePosition(wind, base, initialAngleXY, initialAngleZX, timestep);
    for (int i=1; i < NUM_SEGMENTS; i++)
        segments[i].calculatePosition(wind, segments[i-1].getPosition(), segments[i-1].getAngleXY(), initialAngleZX, timestep);

}

void Grass::draw()
{

    glBegin(GL_QUAD_STRIP);

        glColor3f(0.4*colorShade, colorShade, 0.3*colorShade);

        float sinVal = BASE_WIDTH * cos(DEG2RAD(initialAngleZX));
        float cosVal = BASE_WIDTH * sin(DEG2RAD(initialAngleZX));

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(base.x - 0.5*sinVal, base.y, base.z - 0.5*cosVal);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(base.x + 0.5*sinVal, base.y, base.z + 0.5*cosVal);

        for (int i=0; i < NUM_SEGMENTS; i++)
        {
            Vector3f point = segments[i].getPosition();

            glTexCoord2f(0.0f, (1.0f/NUM_SEGMENTS) * (i+1));
            glVertex3f(point.x - 0.5*sinVal, point.y, point.z - 0.5*cosVal);

            glTexCoord2f(1.0f, (1.0f/NUM_SEGMENTS) * (i+1));
            glVertex3f(point.x + 0.5*sinVal, point.y, point.z + 0.5*cosVal);



        }


    glEnd();


/*
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
        glColor3f(0.0f, 1.0, 0.0f);
        glVertex3f(base.x, base.y, base.z);
        for (int i=0; i < NUM_SEGMENTS; i++)
        {
            Vector3f point = segments[i].getPosition();
            glVertex3f(point.x, point.y, point.z);
        }

    glEnd();

    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0, 0.0f);
        Vector3f point = segments[2].getPosition();
        glVertex3f(base.x, base.y, base.z);
        glVertex3f(point.x, 0.0001f, point.z);


    glEnd();

    for (int i=0; i < NUM_SEGMENTS; i++)
        segments[i].drawLine();
*/
    glBegin(GL_QUAD_STRIP);
        glColor3f(0.2f,0.3f,0.1f);
        glVertex3f(base.x - 0.5*sinVal, 0.0001f, base.z - 0.5*cosVal);
        glVertex3f(base.x + 0.5*sinVal, 0.0001f, base.z + 0.5*cosVal);

        Vector3f point = segments[2].getPosition();
        glVertex3f(point.x - 0.5*sinVal, 0.0001f, point.z - 0.5*cosVal);
        glVertex3f(point.x + 0.5*sinVal, 0.0001f, point.z + 0.5*cosVal);

    glEnd();

    /*glBegin(GL_LINES);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[0][0] + radius[0]*cos(theta0*(3.14159f/180.0f)), points[0][1] + radius[0]*sin(theta0*(3.14159f/180.0f)), points[0][2]);
    glEnd();*/
}
