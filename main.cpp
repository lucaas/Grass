
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cstdlib>
#include <stdlib.h>

#include <vector>

#include "vmath.h"
#include "grass.h"
#include "camera.h"
#include "BMPLoader.h"

// the program should be run standing in the source directory
#define TEXTURE_PATH "data/grass.bmp"

using namespace std;

float windAngle = 0.0f;
float windMagnitude = 0.0f;
double lastTime = 0.0;

vector<Grass *> grasses;

Camera camera;



/* GLUT callback Handlers */
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.5, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    camera.move();

    GLfloat light0_position[] = {15.0, 5.0, 15.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    GLint light0_direction[] = {-2, -1, -2};

    glLightiv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

    //Draw plane
    glBegin(GL_QUADS);
        glColor3f(0.3f,0.5f,0.2f);
        glVertex3f( -10.0f, 0.0f, -10.0f);
        glVertex3f( 10.0f, 0.0f, -10.0f);
        glVertex3f( 10.0f, 0.0f, 10.0f);
        glVertex3f( -10.0f, 0.0f, 10.0f);
    glEnd();

    // Draw grassess

    glLineWidth(3);

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

    //enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    GLfloat light0_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light0_specular[] = {0.25, 0.25, 0.25, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 45);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    float specular_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_color);
    float emission_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_color);

    glClearColor(0.4,0.6,0.9,0.0);




    // Populate the vector with Grass objects
    for (int i=0; i < 5000; i++)
       grasses.push_back(new Grass());



}

// Key handeler
void key (unsigned char key, int x, int y)
{
    camera.key(key, x, y);

    printf("wind: %f\t%f\n", windAngle, windMagnitude);

    // Wind control
    if (key=='0')
        windMagnitude = 0;
    if (key=='8' && windMagnitude < 10)
        windMagnitude += 0.1;
    if (key=='2' && windMagnitude > -10)
        windMagnitude -= 0.1;

    if (key=='4')
        windAngle += 10;
    if (key=='6')
        windAngle -= 10;

    // ESC => Exit
    if (key == 27)
        exit(0);




}
void mouseClick(int button, int state, int x, int y)
{
    camera.mouseClick(button, state, x, y);
}

void mouseMovement(int x, int y) {
    camera.mouseMovement(x, y);
}




static void idle(void)
{



    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

        double timestep;
        if (lastTime == 0.0 || t - lastTime > 1.0)
            timestep = 0.001;
        else
            timestep = t - lastTime;


        lastTime = t;

        vector<Grass *>::iterator  iter = grasses.begin();
        while( iter != grasses.end())
        {
            windMagnitude += 0.0001*rand()/(RAND_MAX) - 0.00005;

            (*iter)->calculate(windAngle, windMagnitude, timestep);
            ++iter;
        }
       // printf("time: %f\n", deltaT);

    glutPostRedisplay();

}

int main(int argc, char *argv[])
{
   srand (time(NULL));

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);



    glutCreateWindow("Grass");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMotionFunc(mouseMovement);
    glutMouseFunc(mouseClick);
    glutIdleFunc(idle);

    BMPClass bmp;
	BMPLoad(TEXTURE_PATH,bmp);


	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); /* or GL_REPLACE */
	glTexImage2D(GL_TEXTURE_2D,0,3,bmp.width,bmp.height,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.bytes);

    setupScene();

    glutMainLoop();

    return EXIT_SUCCESS;
}
