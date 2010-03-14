#include "area.h"

#define HELICOPTER 0
#define NORMAL 1
#define BREEZE 2
#define TORNADO 3


Area::Area()
{
    printf("ERROR!\n");
}

Area::Area(float density, float size, Vector2f center, Terrain *terrain)
{
        init(density, size, center, terrain);
}

void Area::init(float density, float size, Vector2f center, Terrain *terrain)
{
    this->density = density;
    this->size = size;
    this->center = center;

    for(int i = 0; i < MAXGRASS*density; i++)
    {
        float xpos = size*(rand()/float(RAND_MAX)) - size/2;
        float zpos = size*(rand()/float(RAND_MAX)) - size/2;
        float ypos = terrain->getHeight(center.x + xpos, center.y + zpos);
        grasses.push_back(new Grass(xpos, ypos, zpos));
    }
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

