#include<mpi.h>

#include<stdio.h>

int main (int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // if process is master
  if (rank == 0) {
    int numberOfProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    int i;
    for(i=numberOfProcesses-1; i>0; i--) {
      // read the messages and print them
      int number;
      MPI_Status status;
      MPI_Recv(&number, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("Received message %d from source %d.\n", number, i);
    }
  } else {
    // send messages to master
    MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}
