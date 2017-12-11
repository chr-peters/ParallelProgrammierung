/* ---------------------------------------------------------------- */
/* Serielles Program zur Bestimmung des Vorkommens einer Zahl in    */
/* einem Integer-Vektor                                             */
/* ---------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include<mpi.h>

int main (int argc, char **argv){

  MPI_Init(&argc, &argv);

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int *iVector;
  int k;
  int number;
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
    printf("\nk=%d --> %d Processes --> search number = %d\n",k, size, number);

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

  // and broadcast the search number
  MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // determine the size of each receive buffer
  int chunk_size = ceil((float)k / size);
  int *sendcounts = (int *) malloc(size * sizeof(int));
  int *displacement = (int *) malloc(size * sizeof(int));
  for (i = 0; i<size; i++) {
    if (i * chunk_size <= k) {
      sendcounts[i] = chunk_size;
    } else {
      sendcounts[i] = k - i * chunk_size;
    }
    displacement[i] = i * chunk_size;
  }

  // now set chunk_size to the personal chunk_size
  chunk_size = sendcounts[rank];

  // init vector buffer for work
  int *workVector = (int *) malloc(chunk_size * sizeof(int));

  // now receive the vector
  MPI_Scatterv(iVector, sendcounts, displacement,  MPI_INT, workVector, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  /* count appearance of search number */
  for(i=0; i<chunk_size; i++) {
    if(workVector[i] == number) count ++;
  }

  printf("Process %d: number %d --> %d times\n",rank, number, count);

  // now add the results via reduction
  int res;

  MPI_Reduce(&count, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // now print the results
  if (rank == 0) {
    printf("All processes: number %d was found %d times in the array\n", number, res);
    free(iVector);
  }

  free(workVector);
  free(sendcounts);
  free(displacement);

  MPI_Finalize();

  return 0;
}
