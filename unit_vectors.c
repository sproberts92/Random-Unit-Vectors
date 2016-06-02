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
	/* There are better random number generators out there
	 * e.g., MT19937, but in the interests of not having
	 * any dependencies just use the stdlib one here, it's
	 * good enough. */
	return (double)rand()/RAND_MAX;
}

void box_draw(double *v, int d)
{
	/* Draw from a d-cube */
	for (int i = 0; i < d; i++)
		v[i] = 2 * get_rand() - 1;

	/* Determine the length */
	double length = 0;
	for (int i = 0; i < d; i++)
		length += v[i] * v[i];

	length = sqrt(length);

	/* Apply normalisation */
	for (int i = 0; i < d; i++)
		v[i] /= length;
}

void polar_3d(double *v, int d)
{
	/* This only works in d == 3 (although can be extended) */
	(void)d;

	/* Theta is aziumthutal angle, phi is polar angle */
	double theta = 2 * M_PI * get_rand();
	double phi = M_PI * get_rand();

	/* Note, y and z coordinates switched from the
	 * presentation in the post in order to to rotate
	 * the sphere giving a front view instead of a
	 * top view. The front view is more enlightening. */
	v[0] = sin(theta) * cos(phi);
	v[1] = cos(theta);
	v[2] = sin(theta) * sin(phi);

}

void equal_area_proj(double *v, int d)
{
	/* This only works in d == 3 (for now) */
	(void)d;

	/* Theta is aziumuthal angle */
	double theta = 2 * M_PI * get_rand();
	double z = 2 * get_rand() - 1;

	/* From the inverse Lambert equal area projection */
	v[0] = sqrt((1 - z) / 2) * cos(theta);
	v[1] = sqrt((1 - z) / 2) * sin(theta);
	v[2] = z;
}


void discard_corners(double *v, int d)
{
	double length;

	/* Do until we get a point inside the sphere */
	do
	{

		for (int i = 0; i < d; i++)
			v[i] = 2 * get_rand() - 1;

		/* Determine l^2 */
		length = 0;
		for (int i = 0; i < d; i++)
			length += v[i] * v[i];

	} while (length > 1);

	/* For a unit sphere r = r^2, so when comparing the
	 * length in the do-loop we really only need to compare
	 * l^2, save time by taking the square root outside. */
	length = sqrt(length);

	/* Apply normalisation */
	for (int i = 0; i < d; i++)
		v[i] /= length;
}

void generate_and_write(int d, int count, const char* path, void(*generate)(double *v, int d))
{
	printf("Generaring %s...", path);

	/* Watch out, this isn't supported by MSVC, depsite being valid C99. */
	double vec[d];

	FILE* fp = fopen(path, "w");

	for (int k = 0; k < count; ++k)
	{
		/* Generate a vector using the passed generator */
		generate(vec, d);

		/* Write it to a file */
		for (int i = 0; i < d; ++i)
			fprintf(fp, "%lf ", vec[i]);
		fprintf(fp, "\n");
	}

	fclose(fp);

	printf(" Done.\n");
}
