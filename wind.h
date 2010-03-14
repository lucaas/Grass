#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "vmath.h"
#include "grass.h"

#ifndef __WIND_H__
#define __WIND_H__

using namespace std;

class Wind {
public:
Vector2f windCenter;
float windAngle;
float windMagnitude;
static int WINDTYPE;

Wind();
void setWindCenter(Vector2f center);
void setWindType(int type);
Vector2f calculateWind(Vector2f source);
Vector2f calculateHelicopter(Vector2f source);
Vector2f calculateNormal(Vector2f source);
Vector2f calculateBreeze(Vector2f source);
Vector2f calculateTornado(Vector2f source);
void changeWindMagnitude(float step);
void changeWindAngle(float step);
float getWindAngle() { return windAngle; };
float getWindMagnitude() { return windMagnitude; };
Vector2f getWindCenter() { return windCenter; };
int getWindType(){return WINDTYPE;};
void clearWind() { windAngle = 0.0f; windMagnitude = 0.0f; };

};
#endif
