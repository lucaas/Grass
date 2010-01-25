
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

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
    glColor3d(0.5,1,0.5);

    glBegin(GL_QUAD_STRIP);
        glVertex3f(-0.5f, 0.0f, -7.0f);
        glVertex3f( 0.5f, 0.0f, -7.0f);
        glVertex3f(-0.4f, 0.5f, -6.0f);
        glVertex3f( 0.6f, 0.5f, -6.0f);
        glVertex3f( 0.1f, 1.0f, -5.0f);
        glVertex3f( 1.1f, 1.0f, -5.0f);
        glVertex3f( 0.8f, 1.5f, -4.0f);
        glVertex3f( 1.8f, 1.5f, -4.0f);
    glEnd();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
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



    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
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
