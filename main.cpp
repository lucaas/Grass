
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "grass.h"

using namespace std;

float xpos = 5, ypos = 2, zpos = 5, xrot = 0, yrot = -45;
float lastx, lasty;
vector<Grass *> grasses;

void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

/* GLUT callback Handlers */
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    camera();

    GLfloat light0_position[] = {-5.0, 1.0, 5.0, 1.0};
    GLfloat light1_position[] = {5.0, 1.0, -5.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);



    glBegin(GL_QUADS);
        float mcolor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mcolor);
        glVertex3f( -10.0f, 0.0f, -10.0f);
        glVertex3f( 10.0f, 0.0f, -10.0f);
        glVertex3f( 10.0f, 0.0f, 10.0f);
        glVertex3f( -10.0f, 0.0f, 10.0f);
    glEnd();


    glPushMatrix();
        glTranslated(0.0,2.0,0.0);
        glutSolidTeapot(1.0f);
    glPopMatrix();


    // Draw grasses
    vector<Grass *>::iterator  iter = grasses.begin();
    while( iter != grasses.end())
    {
        (*iter)->draw();
        ++iter;
    }


    glutSwapBuffers();
}

void setupScene()
{
    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    /* enable light zero (note: uses default values) */
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    GLfloat light0_diffuse[] = {0.5, 1.0, 0.0, 1.0};
    GLfloat light1_diffuse[] = {1.0, 0.5, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);


    // Populate the vector with Grass objects
    for (int i=0; i < 1000; i++)
        grasses.push_back(new Grass());


}

void key (unsigned char key, int x, int y) {
    if (key=='q')
    {
    xrot += 1;
    if (xrot >360) xrot -= 360;
    }

    if (key=='z')
    {
    xrot -= 1;
    if (xrot < -360) xrot += 360;
    }

    if (key=='w')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos += float(sin(yrotrad)) ;
    zpos -= float(cos(yrotrad)) ;
    ypos -= float(sin(xrotrad)) ;
    }

    if (key=='s')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos -= float(sin(yrotrad));
    zpos += float(cos(yrotrad)) ;
    ypos += float(sin(xrotrad));
    }

    if (key=='d')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos += float(cos(yrotrad)) * 0.2;
    zpos += float(sin(yrotrad)) * 0.2;
    }

    if (key=='a')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= float(cos(yrotrad)) * 0.2;
    zpos -= float(sin(yrotrad)) * 0.2;
    }

    if (key==27)
    {
    exit(0);
    }

    glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y) {
    if(state == GLUT_DOWN)
    {
        lastx=x;
        lasty=y;
    }
}

void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy*0.15; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx*0.15;    //set the xrot to yrot with the addition of the difference in the x position
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);



    glutCreateWindow("Grass");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMotionFunc(mouseMovement);
    glutMouseFunc(mouseClick);
    glutIdleFunc(idle);

    setupScene();

    glutMainLoop();

    return EXIT_SUCCESS;
}
