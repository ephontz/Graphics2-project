#include "Defines.h"

float ILF(Vert _a, Vert _b, float _c, float _d)
{
	return((((int)_a.vector.y - (int)_b.vector.y) * _c) + (((int)_b.vector.x - (int)_a.vector.x) * _d) + ((int)_a.vector.x*(int)_b.vector.y) - ((int)_a.vector.y * (int)_b.vector.x));
}

float LERP(unsigned int _a, unsigned int _b, float r)
{
	return((((int)_b - (int)_a)*r) + _a);
}

unsigned int ColorLERP(unsigned int _start, unsigned int _end, float ratio)
{
	int sA = (_start & 0xFF000000) >> 24;
	int sR = (_start & 0x00FF0000) >> 16;
	int sG = (_start & 0x0000ff00) >> 8;
	int sB = (_start & 0x000000FF);

	int eA = (_end & 0xFF000000) >> 24;
	int eR = (_end & 0x00FF0000) >> 16;
	int eG = (_end & 0x0000ff00) >> 8;
	int eB = (_end & 0x000000FF);

	unsigned int rA = floor((eA - sA)*ratio + sA);
	unsigned int rR = floor((eR - sR)*ratio + sR);
	unsigned int rG = floor((eG - sG)*ratio + sG);
	unsigned int rB = floor((eB - sB)*ratio + sB);

	return (rA << 24 | rR << 16 | rG << 8 | rB);

}

float Deg2Rad(float _degree)
{
	return _degree * (180 / 3.14159);
}

Vec VecTimesMat(Vec _left, Mat _right)
{
	Vec temp;

	temp.x = (_left.x * _right.mat[0][0]) + (_left.y * _right.mat[1][0]) + (_left.z * _right.mat[2][0]) + (_left.w * _right.mat[3][0]);
	temp.y = (_left.x * _right.mat[0][1]) + (_left.y * _right.mat[1][1]) + (_left.z * _right.mat[2][1]) + (_left.w * _right.mat[3][1]);
	temp.z = (_left.x * _right.mat[0][2]) + (_left.y * _right.mat[1][2]) + (_left.z * _right.mat[2][2]) + (_left.w * _right.mat[3][2]);
	temp.w = (_left.x * _right.mat[0][3]) + (_left.y * _right.mat[1][3]) + (_left.z * _right.mat[2][3]) + (_left.w * _right.mat[3][3]);

	return temp;
}

Mat MatTimesMat(Mat _left, Mat _right)
{
	Mat temp;

	temp.mat[0][0] = (_left.mat[0][0] * _right.mat[0][0]) + (_left.mat[0][1] * _right.mat[1][0]) + (_left.mat[0][2] * _right.mat[2][0]) + (_left.mat[0][3] * _right.mat[3][0]);
	temp.mat[0][1] = (_left.mat[0][0] * _right.mat[0][1]) + (_left.mat[0][1] * _right.mat[1][1]) + (_left.mat[0][2] * _right.mat[2][1]) + (_left.mat[0][3] * _right.mat[3][1]);
	temp.mat[0][2] = (_left.mat[0][0] * _right.mat[0][2]) + (_left.mat[0][1] * _right.mat[1][2]) + (_left.mat[0][2] * _right.mat[2][2]) + (_left.mat[0][3] * _right.mat[3][2]);
	temp.mat[0][3] = (_left.mat[0][0] * _right.mat[0][3]) + (_left.mat[0][1] * _right.mat[1][3]) + (_left.mat[0][2] * _right.mat[2][3]) + (_left.mat[0][3] * _right.mat[3][3]);
								   									  										 										
	temp.mat[1][0] = (_left.mat[1][0] * _right.mat[0][0]) + (_left.mat[1][1] * _right.mat[1][0]) + (_left.mat[1][2] * _right.mat[2][0]) + (_left.mat[1][3] * _right.mat[3][0]);
	temp.mat[1][1] = (_left.mat[1][0] * _right.mat[0][1]) + (_left.mat[1][1] * _right.mat[1][1]) + (_left.mat[1][2] * _right.mat[2][1]) + (_left.mat[1][3] * _right.mat[3][1]);
	temp.mat[1][2] = (_left.mat[1][0] * _right.mat[0][2]) + (_left.mat[1][1] * _right.mat[1][2]) + (_left.mat[1][2] * _right.mat[2][2]) + (_left.mat[1][3] * _right.mat[3][2]);
	temp.mat[1][3] = (_left.mat[1][0] * _right.mat[0][3]) + (_left.mat[1][1] * _right.mat[1][3]) + (_left.mat[1][2] * _right.mat[2][3]) + (_left.mat[1][3] * _right.mat[3][3]);
												   																				 										
	temp.mat[2][0] = (_left.mat[2][0] * _right.mat[0][0]) + (_left.mat[2][1] * _right.mat[1][0]) + (_left.mat[2][2] * _right.mat[2][0]) + (_left.mat[2][3] * _right.mat[3][0]);
	temp.mat[2][1] = (_left.mat[2][0] * _right.mat[0][1]) + (_left.mat[2][1] * _right.mat[1][1]) + (_left.mat[2][2] * _right.mat[2][1]) + (_left.mat[2][3] * _right.mat[3][1]);
	temp.mat[2][2] = (_left.mat[2][0] * _right.mat[0][2]) + (_left.mat[2][1] * _right.mat[1][2]) + (_left.mat[2][2] * _right.mat[2][2]) + (_left.mat[2][3] * _right.mat[3][2]);
	temp.mat[2][3] = (_left.mat[2][0] * _right.mat[0][3]) + (_left.mat[2][1] * _right.mat[1][3]) + (_left.mat[2][2] * _right.mat[2][3]) + (_left.mat[2][3] * _right.mat[3][3]);
												   										  										 										
	temp.mat[3][0] = (_left.mat[3][0] * _right.mat[0][0]) + (_left.mat[3][1] * _right.mat[1][0]) + (_left.mat[3][2] * _right.mat[2][0]) + (_left.mat[3][3] * _right.mat[3][0]);
	temp.mat[3][1] = (_left.mat[3][0] * _right.mat[0][1]) + (_left.mat[3][1] * _right.mat[1][1]) + (_left.mat[3][2] * _right.mat[2][1]) + (_left.mat[3][3] * _right.mat[3][1]);
	temp.mat[3][2] = (_left.mat[3][0] * _right.mat[0][2]) + (_left.mat[3][1] * _right.mat[1][2]) + (_left.mat[3][2] * _right.mat[2][2]) + (_left.mat[3][3] * _right.mat[3][2]);
	temp.mat[3][3] = (_left.mat[3][0] * _right.mat[0][3]) + (_left.mat[3][1] * _right.mat[1][3]) + (_left.mat[3][2] * _right.mat[2][3]) + (_left.mat[3][3] * _right.mat[3][3]);

	return temp;
}

Mat RotateX(Mat _left, float _rad)
{
	Mat rot;

	rot.mat[0][0] = 1;
	rot.mat[0][1] = 0;
	rot.mat[0][2] = 0;
	rot.mat[0][3] = 0;

	rot.mat[1][0] = 0;
	rot.mat[1][1] = cos(_rad);
	rot.mat[1][2] = -sin(_rad);
	rot.mat[1][3] = 0;

	rot.mat[2][0] = 0;
	rot.mat[2][1] = sin(_rad);
	rot.mat[2][2] = cos(_rad);
	rot.mat[2][3] = 0;

	rot.mat[3][0] = 0;
	rot.mat[3][1] = 0;
	rot.mat[3][2] = 0;
	rot.mat[3][3] = 1;

	return(MatTimesMat(_left, rot));
}

Mat RotateY(Mat _left, float _rad)
{
	Mat rot;

	rot.mat[0][0] = cos(_rad);
	rot.mat[0][1] = 0;
	rot.mat[0][2] = sin(_rad);
	rot.mat[0][3] = 0;

	rot.mat[1][0] = 0;
	rot.mat[1][1] = 1;
	rot.mat[1][2] = 0;
	rot.mat[1][3] = 0;

	rot.mat[2][0] = -sin(_rad);
	rot.mat[2][1] = 0;
	rot.mat[2][2] = cos(_rad);
	rot.mat[2][3] = 0;

	rot.mat[3][0] = 0;
	rot.mat[3][1] = 0;
	rot.mat[3][2] = 0;
	rot.mat[3][3] = 1;

	return(MatTimesMat(_left, rot));
}

Mat RotateZ(Mat _left, float _rad)
{
	Mat rot;

	rot.mat[0][0] = cos(_rad);
	rot.mat[0][1] = -sin(_rad);
	rot.mat[0][2] = 0;
	rot.mat[0][3] = 0;

	rot.mat[1][0] = sin(_rad);
	rot.mat[1][1] = cos(_rad);
	rot.mat[1][2] = 0;
	rot.mat[1][3] = 0;

	rot.mat[2][0] = 0;
	rot.mat[2][1] = 0;
	rot.mat[2][2] = 1;
	rot.mat[2][3] = 0;

	rot.mat[3][0] = 0;
	rot.mat[3][1] = 0;
	rot.mat[3][2] = 0;
	rot.mat[3][3] = 1;

	return(MatTimesMat(_left, rot));
}

Vert Cart2Raster(Vert vertex)
{
	Vert temp;

	temp.vector.x = (vertex.vector.x + 1)*(RASTER_W / 2);
	temp.vector.y = (1 - vertex.vector.y) * (RASTER_H / 2);

	temp.vector.w = vertex.vector.w;
	temp.u = vertex.u;
	temp.v = vertex.v;
	return temp;
}

Mat Ident()
{
	Mat temp;
	temp.mat[0][0] = 1;
	temp.mat[0][1] = 0;
	temp.mat[0][2] = 0;
	temp.mat[0][3] = 0;
	temp.mat[1][0] = 0;
	temp.mat[1][1] = 1;
	temp.mat[1][2] = 0;
	temp.mat[1][3] = 0;
	temp.mat[2][0] = 0;
	temp.mat[2][1] = 0;
	temp.mat[2][2] = 1;
	temp.mat[2][3] = 0;
	temp.mat[3][0] = 0;
	temp.mat[3][1] = 0;
	temp.mat[3][2] = 0;
	temp.mat[3][3] = 1;

	return temp;
}




Mat InverseDirty(Mat m)
{

	Mat temp;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			temp.mat[i][j] = 0;
		}
	}

	temp.mat[0][0] = m.mat[0][0];
	temp.mat[1][0] = m.mat[0][1];
	temp.mat[2][0] = m.mat[0][2];

	temp.mat[0][1] = m.mat[1][0];
	temp.mat[1][1] = m.mat[1][1];
	temp.mat[2][1] = m.mat[1][2];

	temp.mat[0][2] = m.mat[2][0];
	temp.mat[1][2] = m.mat[2][1];
	temp.mat[2][2] = m.mat[2][2];

	temp.mat[3][3] = 1;

	Vec temp2;
	temp2.x = m.mat[3][0];
	temp2.y = m.mat[3][1];
	temp2.z = m.mat[3][2];
	temp2.w = 0;

	Vec temp3 = VecTimesMat(temp2, temp);

	temp3.x *= -1;
	temp3.y *= -1;
	temp3.z *= -1;
	//temp3.w *= -1;

	temp.mat[3][0] = temp3.x;
	temp.mat[3][1] = temp3.y;
	temp.mat[3][2] = temp3.z;
	temp.mat[3][3] = 1;

	return temp;

}

float Degree_to_rad(int degree)
{
	return degree * (3.14159 / 180);
}

unsigned int TwoDOneD(int x, int y, int width)
{
	return (y*width) + x;


}