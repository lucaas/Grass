#include "wind.h"

#define HELICOPTER 0
#define NORMAL 1
#define BREEZE 2
#define TORNADO 3

Wind::Wind()
{
    windCenter = Vector2f(0.0f, 0.0f);
    windAngle = 0.0f;
    windMagnitude = 0.0f;
    windType = HELICOPTER;
}

void Wind::setWindCenter(Vector2f center)
{
    windCenter = center;
}

void Wind::setWindType(int type)
{
    windType = type;
}

Vector2f Wind::calculateWind(Vector2f source)
{
    switch(windType)
    {
        case HELICOPTER:
            return calculateHelicopter(source);
            break;
        case NORMAL:
            return calculateNormal(source);
            break;
        case BREEZE:
            return calculateBreeze(source);
            break;
        case TORNADO:
            return calculateTornado(source);
            break;
        default:
            return calculateNormal(source);
            break;
    }

}

Vector2f Wind::calculateHelicopter(Vector2f source)
{
    float length = (source - windCenter).length();
    source = (source - windCenter);

    source.y *= -1;

    if(source.x > 0.0 && source.y >= 0.0) windAngle = atan(source.y/source.x);
    else if(source.x > 0.0 && source.y < 0.0) windAngle = atan(source.y/source.x) + 2*M_PI;
    else if(source.x < 0.0) windAngle = atan(source.y/source.x) + M_PI;
    else if(source.x == 0.0 && source.y > 0.0) windAngle = M_PI*0.5;
    else if(source.x == 0.0 && source.y < 0.0) windAngle = M_PI*1.5;
    else windAngle = 0;
    windAngle += M_PI/2;

    windMagnitude += 0.0005 - 0.001*rand()/(RAND_MAX);

    return Vector2f(windAngle * (180/M_PI), windMagnitude*(2/(length+0.001f)));
}

Vector2f Wind::calculateNormal(Vector2f source)
{
    windMagnitude += 0.00025 - 0.0005*rand()/(RAND_MAX);

    return Vector2f(windAngle, windMagnitude);
}

Vector2f Wind::calculateBreeze(Vector2f source)
{
    windAngle = sin(source.x/(source.y + 0.000001f));
    windAngle = windAngle * 180/M_PI;
    windMagnitude = 0.125/2 - .125*rand()/(RAND_MAX);

    return Vector2f(windAngle, windMagnitude);
}

Vector2f Wind::calculateTornado(Vector2f source)
{
    float length = (source - windCenter).length();
    source = (source - windCenter);

    source.y *= -1;

    if(source.x > 0.0 && source.y >= 0.0) windAngle = atan(source.y/source.x);
    else if(source.x > 0.0 && source.y < 0.0) windAngle = atan(source.y/source.x) + 2*M_PI;
    else if(source.x < 0.0) windAngle = atan(source.y/source.x) + M_PI;
    else if(source.x == 0.0 && source.y > 0.0) windAngle = M_PI*0.5;
    else if(source.x == 0.0 && source.y < 0.0) windAngle = M_PI*1.5;
    else windAngle = 0;

    windMagnitude += 0.0005 - 0.001*rand()/(RAND_MAX);

    return Vector2f(windAngle * (180/M_PI), windMagnitude*(2/(length+0.001f)));
}

void Wind::changeWindMagnitude(float step)
{
    windMagnitude += step;
}

void Wind::changeWindAngle(float step)
{
    windAngle += step;
}
