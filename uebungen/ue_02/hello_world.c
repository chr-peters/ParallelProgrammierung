#include<mpi.h>

#include<stdio.h>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("My rank: %d, Size of Communicator: %d\n", rank, size);
  // check if process is master
  if (rank==0) {
    printf("Hello world!\n");
  }
  MPI_Finalize();
  return 0;
}
