
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

struct Vertex
{
        float x,y,z;
        float u, v;
        int color;
};


class Terrain{
private:

        // Array of vertices that make up the terrain
        int numVertices;
        Vertex *vertices;
        // Array of indices used to render the terrain
        int numIndices;
        unsigned short *indices;
        // Size of the terrain (ie. 128)
        int size;
        // Scaling factor (works best from 2 and above)
        float scale;
        // Texture tiling factors
        float uTile;
        float vTile;
public:
        Terrain(char* path, int size, float scale, float textureTile);
        ~Terrain();
        bool loadTerrain(char *filename, int size, float scale);
        void calculateHeightfield(unsigned char *hmap);
        void setTextureTiling(float u, float v) { uTile = u; vTile = v; }
        float getHeight(float x, float z);
        void render();
};

#endif
