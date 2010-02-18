
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cstdlib>
#include <stdlib.h>

#include <vector>
#include <algorithm>

#include "vmath.h"
#include "grass.h"
#include "camera.h"
#include "BMPLoader.h"

// the program should be run standing in the source directory
#define GRASS_TEXTURE_PATH "data/alfa.bmp"
#define PLANE_TEXTURE_PATH "data/plane.bmp"


#define HELICOPTER 0
#define NORMAL 1
#define BREEZE 2
#define TORNADO 3

using namespace std;

float windAngle = 0.0f;
float windMagnitude = 0.0f;
int windType = 0;
double lastTime = 0.0;
GLuint grassTexture;
GLuint planeTexture;

Vector2f windCentre = Vector2f(0.0f, 0.0f);

vector<Grass *> grasses;

Camera camera;


Vector2f calculateWindAngle(Vector2f base);

bool compare(const Grass *a, const Grass *b);
GLuint loadTexture(char *texturepath, GLuint channels);



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
    glBindTexture( GL_TEXTURE_2D, planeTexture );
    glBegin(GL_QUADS);
        glColor3f(0.3f,0.5f,0.2f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f( -10.0f, 0.0f, -10.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 10.0f, 0.0f, -10.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 10.0f, 0.0f, 10.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f( -10.0f, 0.0f, 10.0f);
    glEnd();



    // Draw grassess

    glLineWidth(3);



    glBindTexture( GL_TEXTURE_2D, grassTexture );

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
    glDepthFunc(GL_LEQUAL);

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

    glEnable(GL_TEXTURE_2D);

    // alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.4,0.6,0.9,0.0);

    // Populate the vector with Grass objects
    for (int i=0; i < 5000; i++)
       grasses.push_back(new Grass());

    sort(grasses.begin(), grasses.end(), compare);

    grassTexture = loadTexture(GRASS_TEXTURE_PATH, GL_RGBA);
    planeTexture = loadTexture(PLANE_TEXTURE_PATH, GL_RGB);


}

// Key handeler
void key (unsigned char key, int x, int y)
{
    if (key=='w' || key=='a' || key=='s' || key=='d'){
        camera.key(key, x, y);
        sort(grasses.begin(), grasses.end(), compare);
    }

    //dsprintf("wind: %f\t%f\n", windAngle, windMagnitude);

    // Wind control
    if (key=='0'){
        windMagnitude = 0;
        windCentre.x = 0;
        windCentre.y = 0;
    }
    if (key=='8' && windMagnitude < 10)
        windMagnitude += 0.1;
    if (key=='2' && windMagnitude > -10)
        windMagnitude -= 0.1;

    if (key=='4')
        windAngle += 10;
    if (key=='6')
        windAngle -= 10;

    //Välj vindtyp.. Fixa lite Radiobuttons eller ngt
    if (key=='h')
        windType = HELICOPTER;
    if (key=='t')
        windType = TORNADO;
    if (key=='n')
    {
        windAngle = 0.0f;
        windType = NORMAL;
    }
    if (key=='b')
        windType = BREEZE;

    if (key=='1')
        windCentre.x -= 0.1;
    if (key=='3')
        windCentre.x += 0.1;
    if (key=='7')
        windCentre.y -= 0.1;
    if (key=='9')
        windCentre.y += 0.1;

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
            //Lägger på randomtal på vindstyrkan

            Vector2f base = (*iter)->getBase();
            Vector2f windData = calculateWindAngle(base);

            (*iter)->calculate(windData.x, windData.y, timestep);
            ++iter;
        }
       // printf("time: %f\n", deltaT);

    glutPostRedisplay();

}

//Beräkna vindens riktning och styrka för varje grässtrå
Vector2f calculateWindAngle(Vector2f base)
{
    if(windType == HELICOPTER)
    {

        float length = (base - windCentre).length();
        base = (base - windCentre);

        if(base.x <= 0) windAngle = asin(base.y/length);
        else windAngle = -asin(base.y/length) + M_PI;
        windAngle = windAngle - M_PI/2;
        windMagnitude += 0.0005 - 0.001*rand()/(RAND_MAX);

        if(length < 0.1) length = 0.1;
        return Vector2f(windAngle * (180/M_PI), windMagnitude*(1/length));
    }
    else if(windType == NORMAL)
    {
        windMagnitude += 0.00025 - 0.0005*rand()/(RAND_MAX);

        return Vector2f(windAngle, windMagnitude);
    }
    else if(windType == BREEZE)
    {
        windAngle = sin(base.x/base.y);
        windAngle = windAngle * 180/M_PI;
        windMagnitude = 0.125/2 - .125*rand()/(RAND_MAX);

        return Vector2f(windAngle, windMagnitude);
    }
    else if(windType == TORNADO)
    {
        float length = (base - windCentre).length();
        base = (base - windCentre);

        if(base.x <= 0) windAngle = asin(base.y/length);
        else windAngle = -asin(base.y/length) + M_PI;
        windMagnitude += 0.0005 - 0.001*rand()/(RAND_MAX);

        if(length < 0.1) length = 0.1;
        return Vector2f(windAngle * (180/M_PI), windMagnitude*(5/(length)));
    }

    return Vector2f(0.0f, 0.0f);
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



    setupScene();

    glutMainLoop();

    return EXIT_SUCCESS;
}


bool compare(const Grass *a, const Grass *b)
{

    Vector3f cameraPos = camera.getPosition();
    Vector3f aDiff = a->getPosition() - cameraPos;
    Vector3f bDiff = b->getPosition() - cameraPos;

    return aDiff.lengthSq() > bDiff.lengthSq();

}

GLuint loadTexture(char *texturepath, GLuint channels)
{
    GLuint texture;

    // texture for grass
    BMPClass bmp;
	BMPLoad(texturepath,bmp);

    // allocate a texture name
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

	//glEnable(GL_TEXTURE_2D);


	// texture without mipmaps
	//glTexImage2D(GL_TEXTURE_2D,0,channels,bmp.width,bmp.height,0,channels,GL_UNSIGNED_BYTE,bmp.bytes);

    // build our texture mipmaps, mipmaps our
    gluBuild2DMipmaps(GL_TEXTURE_2D, channels, bmp.width, bmp.height, channels, GL_UNSIGNED_BYTE, bmp.bytes);


    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); /* or GL_REPLACE */


	return texture;

}


