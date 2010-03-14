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
#include "wind.h"
#include "terrain.h"

#ifndef __AREA_H__
#define __AREA_H__

using namespace std;

class Area {

public:
vector<Grass *> grasses;
float density, size;
Vector2f center;
static int MAXGRASS;

Area();
Area(float density, float size, Vector2f center, Terrain *terrain);

void init(float density, float size, Vector2f center, Terrain *terrain);
void draw();
void calculate(float timestep, Vector2f camera, Wind wind);
Vector2f getCenter() const {return center;}
int getSize(){ return grasses.size(); }

};

#endif
