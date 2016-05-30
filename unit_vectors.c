#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define M_PI acos(-1.0)

double get_rand();
void box_draw(double *v, int d);
void polar_3d(double *v, int d);
void equal_area_proj(double *v, int d);
void discard_corners(double *v, int d);
void generate_and_write(int d, int count, const char* path, void(*generate)(double *v, int d));

int main(void)
{
	srand((unsigned int)time(NULL));

	mkdir("output");
	mkdir("output/data");

	generate_and_write(3, 1000,    "output/data/box_draw_1000.dat",       box_draw);
	generate_and_write(3, 10000,   "output/data/box_draw_10000.dat",      box_draw);
	generate_and_write(3, 1000000, "output/data/box_draw_1000000.dat",    box_draw);
	generate_and_write(3, 2000000, "output/data/box_draw_2000000.dat",    box_draw);

	generate_and_write(3, 2000000, "output/data/polar_3d_2000000.dat",        polar_3d);

	generate_and_write(3, 2000000, "output/data/equal_area_proj_2000000.dat", equal_area_proj);
	generate_and_write(3, 2000000, "output/data/discard_corners_2000000.dat", discard_corners);

	return 0;
}

double get_rand()
{
	return (double)rand()/RAND_MAX;
}

void box_draw(double *v, int d)
{
	for (int i = 0; i < d; i++)
	    v[i] = 2 * get_rand() - 1;

	double length = 0;
	for (int i = 0; i < d; i++)
	    length += v[i] * v[i];

	length = sqrt(length);

	for (int i = 0; i < d; i++)
	    v[i] /= length;
}

void polar_3d(double *v, int d)
{
	(void)d;

	double theta = 2 * M_PI * get_rand();
	double phi = M_PI * get_rand();

	v[0] = sin(theta) * cos(theta);
	v[1] = sin(theta) * sin(theta);
	v[2] = cos(theta);
}

void equal_area_proj(double *v, int d)
{
	(void)d;
	
	double theta = 2 * M_PI * get_rand();
	double z = 2 * get_rand() - 1;

	v[0] = sqrt((1 - z) / 2) * cos(theta);
	v[1] = sqrt((1 - z) / 2) * sin(theta);
	v[2] = z;
}


void discard_corners(double *v, int d)
{
	double length;

	do
	{
		length = 0;

	    for (int i = 0; i < d; i++)
	        v[i] = 2 * get_rand() - 1;

	    for (int i = 0; i < d; i++)
	        length += v[i] * v[i];
	} while (length > 1);

	length = sqrt(length);
	for (int i = 0; i < d; i++)
	    v[i] /= length;
}

void generate_and_write(int d, int count, const char* path, void(*generate)(double *v, int d))
{
	double vec[d];

	FILE* fp = fopen(path, "w");

	for (int k = 0; k < count; ++k)
	{
		generate(vec, d);

		for (int i = 0; i < d; ++i)
			fprintf(fp, "%lf ", vec[i]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}