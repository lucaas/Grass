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

    initialAngle = 80.0f + 20.0*(rand()/float(RAND_MAX)); // 80-100 degrees
	float lengthMultiplier = 0.75 + 0.25*(rand()/float(RAND_MAX)); // 0.75-1 * length

    segments[0] = Segment(base, initialAngle, lengthMultiplier * 0.4f);
    segments[1] = Segment(segments[0].getPosition(), segments[0].getAngle(), lengthMultiplier * 0.25f);
    segments[2] = Segment(segments[1].getPosition(), segments[1].getAngle(), lengthMultiplier * 0.2f);


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
    glBegin(GL_QUAD_STRIP);
        glColor3f(0.2f,0.8f,0.2f);
        glVertex3f(base.x, base.y, base.z-0.02);
        glVertex3f(base.x, base.y, base.z+0.06);

        for (int i=0; i < NUM_SEGMENTS; i++)
        {
            Vector3f point = segments[i].getPosition();
            glVertex3f(point.x, point.y, point.z);

            glVertex3f(point.x, point.y, point.z+(NUM_SEGMENTS-i)*0.02);

        }

    glEnd();
    glBegin(GL_QUAD_STRIP);
        glColor3f(0.2f,0.3f,0.1f);
        glVertex3f(base.x, 0.0001f, base.z-0.02);
        glVertex3f(base.x, 0.0001f, base.z+0.06);

        Vector3f point = segments[2].getPosition();
        glVertex3f(point.x, 0.0001f, point.z);
        glVertex3f(point.x, 0.0001f, point.z+0.02);

    glEnd();
    /*glBegin(GL_LINES);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f(points[0][0] + radius[0]*cos(theta0*(3.14159f/180.0f)), points[0][1] + radius[0]*sin(theta0*(3.14159f/180.0f)), points[0][2]);
    glEnd();*/
}
