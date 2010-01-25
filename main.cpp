
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;

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
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,0);

    glLoadIdentity();
    camera();

    glPushMatrix();
        glRotated(-1*t,0,0,1);
        glBegin(GL_LINES);
            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(0.0f, 0.0f, -10.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, 0.5f, -10.0f);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glRotated(-12*t,0,0,1);
        glBegin(GL_LINES);
            glVertex3f(0.0f, 0.0f, -10.0f);
            glVertex3f(0.7f, 0.7f, -10.0f);
        glEnd();
    glPopMatrix();

    glutSwapBuffers();
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

void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy*0.1; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx*0.1;    //set the xrot to yrot with the addition of the difference in the x position
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Grass");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMotionFunc(mouseMovement);
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
