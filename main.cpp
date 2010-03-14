
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

#include "wind.h"
#include "vmath.h"
#include "grass.h"
#include "camera.h"
#include "BMPLoader.h"
#include "BMPWriter.h"
#include "terrain.h"
#include "area.h"

// the program should be run standing in the source directory
#define GRASS_TEXTURE_PATH "data/alfa.bmp"
#define PLANE_TEXTURE_PATH "data/plane.bmp"
#define SKY_TEXTURE_PATH "data/skybox_texture.bmp"
#define DENSITYMAP_PATH "data/densitymap.bmp"


    #define HEIGHTMAP_PATH "data/heightmap.bmp"

#define HELICOPTER 0
#define NORMAL 1
#define BREEZE 2
#define TORNADO 3

int WINDOW_WIDTH;
int WINDOW_HEIGHT;

int HEIGHTMAP_SIZE;
float HEIGHTMAP_SCALE;

int DENSITYMAP_SIZE;
float DENSITYMAP_SCALE;

int Area::MAXGRASS;
int Wind::WINDTYPE;
int Grass::NUM_SEGMENTS;
int Terrain::MAX_HEIGHT;
float Grass::BASE_WIDTH;

using namespace std;
float DO_MOVIE;
float windAngle = 0.0f;
float windMagnitude = 0.0f;
int windType, k, fps = 0;
char title [30];
double lastTime = 0.0;
int framecount = 0;
GLuint grassTexture;
GLuint planeTexture;
GLuint skyTexture;

Vector2f windCentre = Vector2f(0.0f, 0.0f);

vector<Grass *> grasses;
vector<Area *> areas;
int numGrasses = 0;

Wind wind;

Camera camera;

Terrain *terrain;

Vector2f calculateWindAngle(Vector2f base);

void sortGrasses();
bool compareArea(const Area *a, const Area *b);
bool compareGrass(const Grass *a, const Grass *b);
GLuint loadTexture(char *texturepath, GLuint channels);
void RenderSkybox(Vector3f position,Vector3f size,GLuint *SkyBox);



static void display(void)
{

    //camera.xpos = 0.001f;
    //camera.xpos += 0.01f;
    if(DO_MOVIE)
    {
        camera.zpos -= 0.03f;
        camera.yrot -= 0.08f;
        sortGrasses();
    }

   // wind.windCenter.x += 0.06;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //RenderSkybox(camera.getPosition(), Vector3f(100,100,100), &skyTexture);


    glLoadIdentity();
    camera.move();

    GLfloat light0_position[] = {0.6*HEIGHTMAP_SCALE*HEIGHTMAP_SIZE, 10.0, 0.6*HEIGHTMAP_SCALE*HEIGHTMAP_SIZE, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    GLint light0_direction[] = {-2, -1, -2};

    glLightiv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);



    glBindTexture( GL_TEXTURE_2D, planeTexture );

    glColor3f(0.3f,0.5f,0.2f);
    terrain->render();


    glBindTexture( GL_TEXTURE_2D, grassTexture );

    vector<Area *>::iterator  iter = areas.begin();
    while( iter != areas.end())
    {
        (*iter)->draw();
        ++iter;
    }

    glutSwapBuffers();

    // make lots of bmp files of all frames in out folder.
    if (DO_MOVIE)
    {
        glReadBuffer(GL_BACK);
        GLvoid *imageData = malloc(WINDOW_WIDTH*WINDOW_HEIGHT*(8+8+8)); //Allocate memory for storing the image
        glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        char out[100];
        sprintf(out, "out/frame%04i.bmp", framecount++);
        bmp_write(out,(unsigned char*)imageData, WINDOW_WIDTH, WINDOW_HEIGHT);
        if(framecount > 600) exit(0);
    }

}

void setupScene()
{


    //enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    GLfloat light0_ambient[] = {0.1, 0.1, 0.1, 1.0};
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
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Generate terrain
    terrain = new Terrain(HEIGHTMAP_PATH, HEIGHTMAP_SIZE, HEIGHTMAP_SCALE, 1.0f);

    wind = Wind();

    numGrasses = 0;

    BMPClass *densitybmp = new BMPClass();
	BMPLoad(DENSITYMAP_PATH, *densitybmp);

	float area_size = HEIGHTMAP_SIZE*HEIGHTMAP_SCALE/(DENSITYMAP_SCALE*DENSITYMAP_SIZE);

	for (int y=1; y < densitybmp->height -1 ; y++)
    {
        for (int x=1; x < densitybmp->width -1; x++)
        {
            float i = area_size*x-0.5f*HEIGHTMAP_SIZE*HEIGHTMAP_SCALE + 0.5f*area_size;
            float j = area_size*y-0.5f*HEIGHTMAP_SIZE*HEIGHTMAP_SCALE + 0.5f*area_size;
            float density = densitybmp->bytes[y*densitybmp->width + x] / 255.0f;
            areas.push_back(new Area(density, area_size, Vector2f(i, j), terrain));
            numGrasses += (areas.back())->getSize();
        }
    }

    sortGrasses();

    grassTexture = loadTexture(GRASS_TEXTURE_PATH, GL_RGBA);
    planeTexture = loadTexture(PLANE_TEXTURE_PATH, GL_RGB);
    skyTexture = loadTexture(SKY_TEXTURE_PATH, GL_RGB);

}






static void idle(void)
{

    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    if(t>k){
        sprintf(title,"Grass - [ FPS: %i | OBJ: %i ]", fps, numGrasses);
        glutSetWindowTitle(title);
        k++;
        fps = 0;
    }

        double timestep;
        if (!DO_MOVIE)
        {

            if (lastTime == 0.0 || t - lastTime > 0.03)
                timestep = 0.03;
            else
                timestep = t - lastTime;
        }
        else
            timestep = 0.033;

        //ITERATOR FÖR AREAS
        vector<Area *>::iterator iter = areas.begin();
        while( iter != areas.end())
        {
            (*iter)->calculate(timestep, Vector2f(camera.getPosition().x, camera.getPosition().z), wind);
            ++iter;
        }

    glutPostRedisplay();
    fps++;

}

bool compareArea(const Area *a, const Area *b)
{
    Vector2f cameraPos = Vector2f(camera.getPosition().x, camera.getPosition().z);

    Vector2f aDiff = a->getCenter() - cameraPos;
    Vector2f bDiff = b->getCenter() - cameraPos;

    return aDiff.lengthSq() > bDiff.lengthSq();

}

bool compareGrass(const Grass *a, const Grass *b)
{
    Vector2f cameraPos = Vector2f(camera.getPosition().x, camera.getPosition().z);

    Vector2f aDiff = a->getAreaBase() - cameraPos;
    Vector2f bDiff = b->getAreaBase() - cameraPos;

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

/************************************************************************/
/*	Render a skybox with center point position and dimension sizes size */
/************************************************************************/
void RenderSkybox(Vector3f position,Vector3f size,GLuint *SkyBox)
{
// djoubert187 _at_ hotmail.com
	// Begin DrawSkybox
	glDisable(GL_LIGHTING);
	glColor4f(1.0, 1.0, 1.0, 1.0f);

	// Save Current Matrix
	glPushMatrix();

	// Second Move the render space to the correct position (Translate)
	glTranslatef(position.x,position.y,position.z);

	// First apply scale matrix
	glScalef(size.x,size.y,size.z);

	float cz = -0.0f,cx = 1.0f;
	float r = 1.0f; // If you have border issues change this to 1.005f
	// Common Axis Z - FRONT Side
	glBindTexture(GL_TEXTURE_2D,SkyBox[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(cx, cz); glVertex3f(-r ,1.0f,-r);
		glTexCoord2f(cx,  cx); glVertex3f(-r,1.0f,r);
		glTexCoord2f(cz,  cx); glVertex3f( r,1.0f,r);
		glTexCoord2f(cz, cz); glVertex3f( r ,1.0f,-r);
	glEnd();

	// Common Axis Z - BACK side

	glBegin(GL_QUADS);
		glTexCoord2f(cx,cz);  glVertex3f(-r,-1.0f,-r);
		glTexCoord2f(cx,cx);  glVertex3f(-r,-1.0f, r);
		glTexCoord2f(cz,cx);  glVertex3f( r,-1.0f, r);
		glTexCoord2f(cz,cz);  glVertex3f( r,-1.0f,-r);
	glEnd();

	// Common Axis X - Left side
	glBegin(GL_QUADS);
		glTexCoord2f(cx,cx); glVertex3f(-1.0f, -r, r);
		glTexCoord2f(cz,cx); glVertex3f(-1.0f,  r, r);
		glTexCoord2f(cz,cz); glVertex3f(-1.0f,  r,-r);
		glTexCoord2f(cx,cz); glVertex3f(-1.0f, -r,-r);
	glEnd();

	// Common Axis X - Right side
	glBegin(GL_QUADS);
		glTexCoord2f( cx,cx); glVertex3f(1.0f, -r, r);
		glTexCoord2f(cz, cx); glVertex3f(1.0f,  r, r);
		glTexCoord2f(cz, cz); glVertex3f(1.0f,  r,-r);
		glTexCoord2f(cx, cz); glVertex3f(1.0f, -r,-r);
	glEnd();

	// Common Axis Y - Draw Up side
	glBegin(GL_QUADS);
		glTexCoord2f(cz, cz); glVertex3f( r, -r,1.0f);
		glTexCoord2f(cx, cz); glVertex3f( r,  r,1.0f);
		glTexCoord2f(cx, cx); glVertex3f(-r,  r,1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,1.0f);
	glEnd();

	// Common Axis Y - Down side
	glBegin(GL_QUADS);
		glTexCoord2f(cz,cz);  glVertex3f( r, -r,-1.0f);
		glTexCoord2f( cx,cz); glVertex3f( r,  r,-1.0f);
		glTexCoord2f( cx,cx); glVertex3f(-r,  r,-1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,-1.0f);
	glEnd();

	// Load Saved Matrix
	glPopMatrix();
    glEnable(GL_LIGHTING);

};

void sortGrasses()
{
    sort(areas.begin(), areas.end(), compareArea);
    vector<Area *>::iterator iter = areas.begin();
    while(iter != areas.end())
    {
        sort((*iter)->grasses.begin(), (*iter)->grasses.end(), compareGrass);
        iter++;
    }
}

/* ----------------- GLUT callback Handlers */
void key (unsigned char key, int x, int y)
{
    if (key=='w' || key=='a' || key=='s' || key=='d'){
        camera.key(key, x, y);
        sortGrasses();
    }

    //dsprintf("wind: %f\t%f\n", windAngle, windMagnitude);

    // Wind control
    if (key=='0'){
        wind.clearWind();
    }
    if (key=='8' && wind.getWindMagnitude() < 10)
        wind.changeWindMagnitude(0.1f);

    if (key=='2' && wind.getWindMagnitude() > -10)
        wind.changeWindMagnitude(-0.1f);

    if (key=='4')
        wind.changeWindAngle(10.0f);
    if (key=='6')
        wind.changeWindAngle(-10.0f);


    //Välj vindtyp.. Fixa lite Radiobuttons eller ngt
    if (key=='h')
        wind.setWindType(HELICOPTER);
    if (key=='t')
        wind.setWindType(TORNADO);
    if (key=='n')
    {
        wind.clearWind();
        wind.setWindType(NORMAL);
    }
    if (key=='b')
        wind.setWindType(BREEZE);

    if (key=='1')
    {
        Vector2f tempCenter = wind.getWindCenter();
        tempCenter.x -= 0.1;
        wind.setWindCenter(tempCenter);
    }
    if (key=='3')
    {
        Vector2f tempCenter = wind.getWindCenter();
        tempCenter.x += 0.1;
        wind.setWindCenter(tempCenter);
    }
    if (key=='7')
    {
        Vector2f tempCenter = wind.getWindCenter();
        tempCenter.y -= 0.1;
        wind.setWindCenter(tempCenter);
    }
    if (key=='9')
    {
        Vector2f tempCenter = wind.getWindCenter();
        tempCenter.y += 0.1;
        wind.setWindCenter(tempCenter);
    }

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

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.5, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
/* ----------------- */


int main(int argc, char *argv[])
{
    vector<float> settings;
    float value;
    string line;
    ifstream myfile ("data/config.txt");
    if (myfile.is_open())
    {
        while (myfile >> line && myfile >> value)
        {
            cout << line << " " << value << endl;
            settings.push_back(value);
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    Area::MAXGRASS = settings[0]/(settings[9]*settings[9]);
    DO_MOVIE = settings[1];
    Wind::WINDTYPE = settings[2];
    Grass::NUM_SEGMENTS = settings[3];
    Grass::BASE_WIDTH = settings[4];
    WINDOW_WIDTH = settings[5];
    WINDOW_HEIGHT = settings[6];
    HEIGHTMAP_SIZE = settings[7];
    HEIGHTMAP_SCALE = settings[8];
    DENSITYMAP_SIZE = settings[9];
    DENSITYMAP_SCALE = settings[10];
    Terrain::MAX_HEIGHT = settings[11];

    srand (time(NULL));

    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
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


