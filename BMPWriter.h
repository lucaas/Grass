#ifndef __BMP_WRITER__
#define __BMP_WRITER__

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

typedef unsigned char uint08;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef struct
{
	uint16 identifier;
	uint32 file_size;
	uint16 a;
	uint16 b;
	uint32 pixels_offset;
	uint32 header_size;
	uint32 width;
	uint32 height;
	uint16 colors;
	uint16 bit_depth;
	uint32 compression_method;
	uint32 pixel_count;
	uint32 resolution_h;
	uint32 resolution_v;
	uint32 palette_colors;
	uint32 important_colors;
} bmp_header;

void bmp_write(const char* filename, uint08* pixels, uint32 width, uint32 height)
{
	fstream stream;
	uint32 offset;

	bmp_header header = { 19778, 54+3*width*height, 0, 0, 54, 40, width, height, 1, 24, 0, width*height, 1, 1, 0, 0 };

	stream.open(filename, ios::binary | ios::out);

	stream.write((char*)&header.identifier, 2);
	stream.write((char*)&header.file_size, 4);
	stream.write((char*)&header.a, 2);
	stream.write((char*)&header.b, 2);
	stream.write((char*)&header.pixels_offset, 4);
	stream.write((char*)&header.header_size, 4);
	stream.write((char*)&header.width, 4);
	stream.write((char*)&header.height, 4);
	stream.write((char*)&header.colors, 2);
	stream.write((char*)&header.bit_depth, 2);
	stream.write((char*)&header.compression_method, 4);
	stream.write((char*)&header.pixel_count, 4);
	stream.write((char*)&header.resolution_h, 4);
	stream.write((char*)&header.resolution_v, 4);
	stream.write((char*)&header.palette_colors, 4);
	stream.write((char*)&header.important_colors, 4);

	for (uint32 y = 0; y < height; y++)
	{
		for (uint32 x = 0; x < width; x++)
		{
			offset = 3*(y*width+x);

			stream.write((char*)&pixels[offset+2], 1);
			stream.write((char*)&pixels[offset+1], 1);
			stream.write((char*)&pixels[offset+0], 1);
		}
	}

	stream.close();
}
/*
int main(int argc, char** argv)
{
	char* filename = "fuckme.bmp";
	uint08* pixels;
	uint32 height = 128;
	uint32 width = 128;

	pixels = new uint08[3*height*width];

	for (uint32 i = 0; i < width*height; i++)
	{
		pixels[i*3+0] = (uint08)(255*((double)rand()/RAND_MAX));
		pixels[i*3+1] = (uint08)(255*((double)rand()/RAND_MAX));
		pixels[i*3+2] = (uint08)(255*((double)rand()/RAND_MAX));
	}

	bmp_write(filename, pixels, width, height);

	delete [] pixels;
}
*/


#endif
