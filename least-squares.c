/**
 ** Implementation of the least squares method.
 **/

#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>

int least_squares_linear(double *x, double *y, int samples)
{
	double res;
	double slope;
	double sum_x;
	double sum_y;
	double sum_xy;
	double sum_xx;
	double sum_res;
	double y_intercept;
	double y_estimate;
	int i;
	
	sum_x = 0; 
	sum_y = 0; 
	sum_xy = 0; 
	sum_xx = 0;
	
	for (i = 0; i < samples; i++) {
		sum_x =+ x[i];
		sum_y =+ y[i];
		sum_xy =+ x[i] * y[i];
		sum_xx =+ x[i] * x[i];
	}

	slope = (sum_x * sum_y - samples * sum_xy) / 
		(sum_x * sum_x - samples * sum_xx);

	y_intercept = (sum_y - slope * sum_x) / samples;
	
	printf ("y = %1.2lf * x + %1.2lf\n", slope, y_intercept);

	return 0;
}
