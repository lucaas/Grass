#include "grass.h"
#include "segment.h"

Grass::Grass()
{

    // Random nummer mellan -10 och 10
    float xpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;
    float zpos = 20.0*(rand()/float(RAND_MAX)) - 10.0;

    Grass(xpos,zpos);
}

Grass::Grass(float x, float z)
{
    base.x = x;
    base.y = 0.0f;
    base.z = z;

    initialAngle = 90.0f;

    segments[0] = Segment(base, initialAngle);
    segments[1] = Segment(segments[0].getPosition(), segments[0].getAngle());
    segments[2] = Segment(segments[1].getPosition(), segments[1].getAngle());


}

Grass::~Grass()
{
    delete segments;
}

void Grass::calculate(Vector3f wind, float timestep)
{
    segments[0].calculatePosition(wind, base, initialAngle, timestep);
    for (int i=1; i < NUM_SEGMENTS; i++)
        segments[i].calculatePosition(wind, segments[i-1].getPosition(), segments[i-1].getAngle(), timestep);

}

void Grass::draw()
{
    glBegin(GL_LINE_STRIP);
        glVertex3f(base.x, base.y, base.z);
        for (int i=0; i < NUM_SEGMENTS; i++)
        {
            Vector3f point = segments[i].getPosition();
            glVertex3f(point.x, point.y, point.z);
        }
    glEnd();

    /*glBegin(GL_LINES);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[0][0] + radius[0]*cos(theta0*(3.14159f/180.0f)), points[0][1] + radius[0]*sin(theta0*(3.14159f/180.0f)), points[0][2]);
    glEnd();*/
}
