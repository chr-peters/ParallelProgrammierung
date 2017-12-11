#include<mpi.h>

#include<stdlib.h>

int main (int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int count;
  MPI_Comm_size(MPI_COMM_WORLD, &count);

  int right_neighbor = (rank + 1)%count;
  int left_neighbor = (rank - 1 + count)%count;

  int i;
  // first send start message
  MPI_Request start_request;
  MPI_Isend(&rank, 1, MPI_INT, right_neighbor, 0, MPI_COMM_WORLD, &start_request);

  int message;

  for (i=0; i<count; i++) {
    // receive message from left neighbor
    MPI_Request request;
    MPI_Irecv(&message, 1, MPI_INT, left_neighbor, MPI_ANY_TAG, MPI_COMM_WORLD, &request);

    // wait for it...
    MPI_Status status;
    MPI_Wait(&request, &status);

    // add the rank
    message += rank;

    // send the message to the right neighbor
    MPI_Isend(&message, 1, MPI_INT, right_neighbor, 0, MPI_COMM_WORLD, &request);
  }

  // restore the old value
  message -= rank;

  // print the result
  printf("Hello, I am %d and my sum is %d.\n", rank, message);

  MPI_Finalize();
  return 0;
}
