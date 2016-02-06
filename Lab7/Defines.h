//#include <iostream>
//using namespace std;
//#include "RasterSurface.h"
#include <algorithm>
//#include "greendragon.h"
#include <Windows.h>


#define RASTER_W 500
#define RASTER_H 500
#define NUM_PIX (RASTER_W * RASTER_H)
//unsigned int raster[RASTER_W][RASTER_H];
//float ZBuffer[RASTER_W][RASTER_H];




struct Vec
{
	float x, y, z, w;
};

struct Vert
{
	Vec vector;
	unsigned int color;
	float u = 0,v = 0;
};

struct Mat
{
	float mat[4][4];
};

struct point
{
	unsigned int x, y;
};