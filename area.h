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

#ifndef __AREA_H__
#define __AREA_H__

using namespace std;

class Area {

private:
vector<Grass *> grasses;
float density, size;
Vector2f center;

public:

Area();
Area(float density, float size, Vector2f center);

void init(float density, float size, Vector2f center);
void draw();
void calculate(float timestep, Vector2f camera, Wind wind);
Vector2f getCenter() const {return center;};

};

#endif
