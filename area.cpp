#include "area.h"

#define MAXGRASS 50
#define HELICOPTER 0
#define NORMAL 1
#define BREEZE 2
#define TORNADO 3


Area::Area()
{
    init(0.5, 5, Vector2f(0.0f, 0.0f));
}

Area::Area(float density, float size, Vector2f center)
{
    init(density, size, center);
}

void Area::init(float density, float size, Vector2f center)
{
    this->density = density;
    this->size = size;
    this->center = center;

    for(int i = 0; i < MAXGRASS*density; i++)
        grasses.push_back(new Grass(size));
}

void Area::draw()
{
    glPushMatrix();
        glTranslated(center.x, 0, center.y);
        vector<Grass *>::iterator  iter = grasses.begin();
        while( iter != grasses.end())
        {
            (*iter)->draw();
            ++iter;
        }
    glPopMatrix();
}

void Area::calculate(float timestep, Vector2f camera, Wind wind)
{
    Vector2f windData;
    windData = wind.calculateWind(center);
    vector<Grass *>::iterator  iter = grasses.begin();
    while( iter != grasses.end())
    {
        if((center - wind.getWindCenter()).length() < 5 && (wind.getWindType() == HELICOPTER || wind.getWindType() == TORNADO))
        {
            windData = wind.calculateWind(center + (*iter)->getBase());
        }

        (*iter)->calculate(windData.x, windData.y, timestep);
        ++iter;
    }
}

