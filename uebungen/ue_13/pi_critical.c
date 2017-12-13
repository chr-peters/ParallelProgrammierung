/*  Parallel Program */
/*  Approximate pi with the n-point rectangle quadrature rule */
/*  applied to the integral from 0 to 1 of 4 / (1+x**2).c */

#include <stdio.h>
#include <sys/time.h>

#define MAXN 100000000

double esecs (void){

  struct timeval tv;
  double time;

  gettimeofday(&tv,NULL);
  time = (double) tv.tv_sec + ((double)tv.tv_usec / (1000.0*1000.0));

  return time;
}

int main(int argc, char *argv[])
{
  double pi  = 0.0; /* The calculated result */
  int    n   = MAXN;/* Number of points of integration */
  if (argc > 1) {
    n = atoi(argv[1]);
  }
  double h;         /* Reciprocal of n, interval size */
  double x;         /* Midpoint of each rectangle's interval */
  double f;         /* Function to integrate */
  double sum;       /* Area of rectangles */

  double tstart, tend, time;

  int    i; /* do loop index */

  tstart = esecs();

  h   = 1.0 / n; /* Calculate interval size */
  sum = 0.0;     /* Initialize sum */
  
  /* Calculate partial sums */
#pragma omp parallel for private(x, f)
  for(i = 1; i <= n; i++) {
    x   = (i - 0.5) * h;
    f   = 4.0 / (1.0 + x * x);
#pragma omp critical
    sum = sum + f;
  }
  
  pi = h * sum;
  
  tend = esecs();
  time = tend - tstart;

  printf("%d %f %f\n", n, time, pi);

  return 0;
}
