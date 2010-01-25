
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
    glLoadIdentity();
    camera();

    glBegin(GL_QUAD_STRIP);
        glVertex3f(-0.1f, 0.0f, -7.0f);
        glVertex3f( 0.1f, 0.0f, -7.0f);
        glVertex3f( 0.0f, 0.8f, -6.9f);
        glVertex3f( 0.2f, 0.8f, -6.9f);
        glVertex3f( 0.1f, 1.4f, -6.5f);
        glVertex3f( 0.3f, 1.4f, -6.5f);
        glVertex3f( 0.2f, 1.6f, -6.0f);
        glVertex3f( 0.4f, 1.6f, -6.0f);
    glEnd();


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
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);



    glutCreateWindow("Grass");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMotionFunc(mouseMovement);
    glutMouseFunc(mouseClick);
    glutIdleFunc(idle);

    glShadeModel(GL_FLAT); // flat shading

    glEnable(GL_LIGHTING); // enable lighting

    /* enable light zero (note: uses default values) */
    glEnable(GL_LIGHT0);
    GLfloat light0_diffuse[] = {0.5, 1.0, 0.0, 1.0};
    GLfloat light0_position[] = {-1.0, 0.0, 0.0, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return EXIT_SUCCESS;
}
