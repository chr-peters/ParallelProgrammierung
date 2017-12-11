#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#if defined(_OPENMP)
#include <omp.h>
#endif

#define BLOCKSIZE 10000000
#define MEASUREMENTS 20

#if !defined(_OPENMP)
typedef int omp_sched_t;
#endif

double esecond(void) {

  struct timeval tp;
  struct timezone tzp;
  
  gettimeofday(&tp, &tzp);
  return tp.tv_sec + (tp.tv_usec * 1e-6);
}

int main(int argc, char **argv) {
  char * sched_types[]={"serial", "static", "dynamic", "guided", "auto"};
  static float a[BLOCKSIZE];
  double s, t;
  double start, end;
  int nthreads, i;
  omp_sched_t sched_type;
  int chunksize;
  int sweep;

#if defined(_OPENMP)
  omp_get_schedule(&sched_type, &chunksize);
#else
  sched_type=0;
nthreads=1;
chunksize=BLOCKSIZE;
#endif

  start = esecond();

  for (sweep=0; sweep < MEASUREMENTS; sweep++)
  {

#pragma omp parallel for schedule(runtime)
    for(i=0; i<BLOCKSIZE; i++) {
      a[i] = i*17;
#if defined(_OPENMP)
      if (i==BLOCKSIZE-1){
	nthreads=omp_get_num_threads();
      }
#endif
    }

    s=0.0;

#pragma omp parallel for schedule(runtime) private(t) reduction(+:s)
    for(i=0; i<BLOCKSIZE; i++) {
      t = a[i];
      s = s + sqrt(t);
    }
  } /* end for */

  end = esecond();

  printf("... %10.3f seconds on %d pe(s) (type: %s, chunksize: %d)\n",
         (end - start)/MEASUREMENTS, nthreads, sched_types[sched_type], chunksize);

  return(0);
}

