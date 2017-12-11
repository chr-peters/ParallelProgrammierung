/* ---------------------------------------------------------------- */
/* Serielles Program zur Bestimmung des Vorkommens einer Zahl in    */
/* einem Integer-Vektor                                             */
/* ---------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>

#include<mpi.h>

int main (int argc, char **argv){

  MPI_Init(&argc, &argv);

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int *iVector;
  int k;
  int number = rank;
  int i;
  int count = 0;

  if (rank == 0) {

    FILE *in;
  
    /* initialize */
    in = fopen("input_numarray.dat", "r");
    
    if (in == NULL)
      printf("ERROR: Cannot open file input_numarray.dat!\n");
    else{
      fscanf(in,"%d",&k);
      fscanf(in,"%d",&number);
      fclose(in);
    }

    
    iVector = (int*) malloc (k * sizeof(int));

    /* fill array with random numbers between 0 and 9 */
    printf("\nk=%d --> %d Processes\n",k, size);

    printf("\nArray:\n");
    for(i=0; i<k; i++){
      iVector[i] = rand()%10;
      printf(" %i", iVector[i]);
      if((i+1)%20 == 0) printf("\n");
    }

    printf("\n\n");

  }

  // now broadcast k
  MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // init vector buffer if not master
  if (rank != 0) {
    iVector = (int *) malloc(k * sizeof(int));
  }

  // now receive the vector
  MPI_Bcast(iVector, k, MPI_INT, 0, MPI_COMM_WORLD);

  /* count appearance of search number */
  for(i=0; i<k; i++) {
    if(iVector[i] == number) count ++;
  }

  // now gather the results
  int *res;
  if (rank == 0 ) {
    res = (int *) malloc(size * sizeof(int));
  }
  MPI_Gather(&count, 1, MPI_INT, res, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // now print the results
  if (rank == 0) {
    for (i = 0; i < size; i++ ) {
      printf("Number %d was found %d times in the array\n\n", i, res[i]);
    }
    free(res);
  }

  free(iVector);

  MPI_Finalize();

  return 0;
}
