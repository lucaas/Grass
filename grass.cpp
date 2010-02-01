#include "grass.h"
#include "segment.h"

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

    //initialAngle = 70.0f + 40.0*(rand()/float(RAND_MAX));
    initialAngle = 90.0f;
    segments[0] = Segment(base, initialAngle);
    segments[1] = Segment(segments[0].getPosition(), segments[0].getAngle(), 0.25f);
    segments[2] = Segment(segments[1].getPosition(), segments[1].getAngle(), 0.2f);


}

Grass::~Grass()
{
    delete segments;
}

void Grass::calculate(Vector3f wind, double timestep)
{
    segments[0].calculatePosition(wind, base, initialAngle, timestep);
    for (int i=1; i < NUM_SEGMENTS; i++)
        segments[i].calculatePosition(wind, segments[i-1].getPosition(), segments[i-1].getAngle(), timestep);

}

void Grass::draw()
{
    glBegin(GL_LINE_STRIP);
        glColor3f(0.2f,0.8f,0.2f);
        glVertex3f(base.x, base.y, base.z);
        for (int i=0; i < NUM_SEGMENTS; i++)
        {
            Vector3f point = segments[i].getPosition();
            glVertex3f(point.x, point.y, point.z);
        }

    glEnd();
    glBegin(GL_LINES);
        glColor3f(0.0f,0.0f,0.0f);
        glVertex3f(base.x, base.y, base.z);
        Vector3f point = segments[2].getPosition();
        glVertex3f(point.x, 0.0f, point.z);
    glEnd();
    /*glBegin(GL_LINES);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[0][0] + radius[0]*cos(theta0*(3.14159f/180.0f)), points[0][1] + radius[0]*sin(theta0*(3.14159f/180.0f)), points[0][2]);
    glEnd();*/
}
