/*  Serial Program */
/*  Approximate pi with the n-point rectangle quadrature rule */
/*  applied to the integral from 0 to 1 of 4 / (1+x**2).c */

#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  FILE *in;
  
  double pi  = 0.0; /* The calculated result */
  int    n   = 0;   /* Number of points of integration */
  double h;         /* Reciprocal of n, interval size */
  double x;         /* Midpoint of each rectangle's interval */
  double f;         /* Function to integrate */
  double sum;       /* Area of rectangles */

  double times [8];

  int    i; /* do loop index */
  int j;

  for (j=1; j<=8; j++) {

    // Barrier for time measurement
    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    n = pow(10, j);

    if(n > 0) {
    
      h   = 1.0 / n; /* Calculate interval size */
      sum = 0.0;     /* Initialize sum */
      int intervals_per_process = ceil((double)n / size);
      int start = intervals_per_process * rank;
    
      /* Calculate partial sums */
      for(i = 1; i <= intervals_per_process; i++) {
	x   = (start + i - 0.5) * h;
	if (x > 1) {
	  break;
	}
	f   = 4.0 / (1.0 + x * x);
	sum = sum + f;
      }

      // now gather the result
      MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

      if (rank == 0) {
	pi = h * pi;
      }

      double end = MPI_Wtime() - t_start;
      double max_time;
      MPI_Reduce(&end, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

      if (rank == 0) {
	times[j-1] = max_time;
      }
    }
    else {
      if (rank == 0) {
	printf("ERROR: %d is not a valid value for n. n must be > 0\n",n);
      }
      MPI_Abort(MPI_COMM_WORLD, 0);
    }
  }

  // now print the times
  if (rank == 0) {
    printf("%f %f %f %f %f %f %f %f\n", times[0], times[1], times[2], times[3],
	   times[4], times[5], times[6], times[7]);
  }
  return 0;
}
