#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define BLEN 10
#define DATA_TAG 99

int main (int argc, char **argv)
{

    int i, j;
    int *data, *buffer;
    int my_rank, num_procs;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    MPI_Datatype custom_type;
    MPI_Type_vector(BLEN, 1, num_procs, MPI_INT, &custom_type);
    MPI_Type_commit(&custom_type);

    MPI_Request *requests;

    /* MASTER PART */
    if(my_rank == 0){

      /* allocate memory */
      data = (int*) malloc (BLEN * num_procs * sizeof(int));

      requests = (MPI_Request *)malloc(num_procs * sizeof(MPI_Request));

      /* initialize array */
      for(i=0; i<BLEN*num_procs; ++i){
	data[i] = i;
	/*printf(" %d ",data[i]);*/
      }

      // now send the parts
      for (i=0; i<num_procs; i++) {
	MPI_Isend(data+i, 1, custom_type, i, DATA_TAG, MPI_COMM_WORLD, requests+i);
      }

    }

    /* WORKER PART */
    buffer = (int*) malloc (BLEN * sizeof(int));
    MPI_Recv(buffer, BLEN, MPI_INT, 0, DATA_TAG, MPI_COMM_WORLD, &status);
    printf("Process %d recieved: ",my_rank);
    for(j=0; j<BLEN; j++)
      printf(" %d ",buffer[j]);
    printf("\n");

    /* CLEAN UP */
    if (my_rank == 0){
      for(i=0; i<num_procs; i++) {
	MPI_Status curStatus;
	MPI_Wait(requests+i, &curStatus);
      }
      free(data);
      free(requests);
    }
    free(buffer);
    MPI_Type_free(&custom_type);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
