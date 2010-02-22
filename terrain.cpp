#include "terrain.h"
#include "BMPLoader.h"
#include <cstring>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

Terrain::Terrain(char* path, int theSize, int theScale, float textureTile)
{
    size = theSize;
    scale = theScale;
    uTile = textureTile;
    vTile = textureTile;

    BMPClass *bmp = new BMPClass();
	BMPLoad(path, *bmp);

    // calculate the 3D heightfield
    calculateHeightfield(bmp->bytes);

    delete bmp;

}


void Terrain::calculateHeightfield(unsigned char *hmap)
{

       // the number of vertices is the size of the heightmap squared
       numVertices = size*size;
       // allocate memory for the vertices and clear the memory to 0
       vertices = new Vertex[numVertices];
       memset(vertices, 0, numVertices);
       // used for the texture calculation
       float texdelta = 2.0f/size;
       // temporary vertex
       Vertex vert;
       int currVertex=0;
       // loop through every pixel in the heightmap
       for (int y=0; y < size; y++)
    {
         for (int x=0; x < size; x++)
         {
                // Get the current vertex we want to calculate
                currVertex = y*size+x;
                // x, z coordinates are given by this pixel's x, y coords multiplied
                // by a scaling factor (which defines how far away from each other
                // each vertex is on the 3D heightfield).
                vert.x = (float)x * scale - 0.5*size*scale;
                vert.z = (float)y * scale - 0.5*size*scale;
                // calculate the height from the value in the heightmap

                vert.y = (float)(hmap[currVertex] * (scale*3/255.0f));

                // calculate the texture coordinates
                vert.u = uTile * ((float)x * texdelta * 0.5f);
                vert.v = vTile * (1.0f - (float)y * texdelta * 0.5f);

                // set the color to white
                vert.color = 0xffffffff;
                // put this vertex into the vertices array
                vertices[currVertex] = vert;
         }
  }
  // Allocate memory for the indices and clear it
  numIndices = (size-1)*(size-1)*6;
  indices = new unsigned short[numIndices];
  memset(indices, 0, numIndices);
  // Calculate the indices
  int idx=0;
  for (int y=0; y < size-1; y++)
  {
         for (int x=0; x < size-1; x++)
         {
                currVertex = y*(size)+x;
                indices[idx++] = currVertex;
                indices[idx++] = currVertex+1;
                indices[idx++] = currVertex+(size);
                indices[idx++] = currVertex+(size)+1;
                indices[idx++] = currVertex+(size);
                indices[idx++] = currVertex+1;
         }
  }
}

void Terrain::render()
{

       // Enable using Vertex Arrays

       glEnableClientState(GL_VERTEX_ARRAY);
       glEnableClientState(GL_TEXTURE_COORD_ARRAY);
       // Set the Vertex Array to the start of our vertex data
       glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].x);
       glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].u);
       // Render the Terrain
       glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, indices);
       // Disable Vertex Arrays
       glDisableClientState(GL_VERTEX_ARRAY);
       glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


// o-optimerad kod för att hämta y-position (returnerar minsta värdet av de tre närmsta punkterna)
float Terrain::getHeight(float x, float z) {


    float minDistance[] = {1000,1000,1000};
    float minHeight[3];


    for (int i=0; i < size; i++)
    {
        for (int j=0; j < size; j++)
        {
            float distanceX = vertices[i*size+j].x - x;
            float distanceZ = vertices[i*size+j].z - z;

            float distance = distanceX*distanceX + distanceZ*distanceZ;

            for (int k = 0; k < 3; k++)
            {
                if (distance < minDistance[k])
                {
                    float maxDist = 0;
                    int maxPos = 0;
                    for (int m = 0; m < 3; m++)
                    {
                        if (minDistance[m] > maxDist)
                        {
                            maxDist = minDistance[m];
                            maxPos = m;
                        }

                    }
                    minDistance[maxPos] = distance;
                    minHeight[maxPos] = vertices[i*size+j].y;
                    break;
                }
            }
        }
    }

    float height = 10000;
    for ( int k = 0; k < 3; k++)
        if (minHeight[k] < height)
            height = minHeight[k];



    return  height;


}

