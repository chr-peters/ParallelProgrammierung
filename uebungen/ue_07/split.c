#include <mpi.h>
#include <stdio.h>

int main (int argc, char **argv) {

  MPI_Init(&argc, &argv);

  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // now split
  MPI_Comm newComm;
  int color = rank%2;
  MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newComm);

  // get the new rank
  int new_rank;
  MPI_Comm_rank(newComm, &new_rank);

  // send message
  int msg;
  if (new_rank == 0 && color == 0)
    msg = 4711;
  else if (new_rank == 0 && color == 1)
    msg = 23;

  MPI_Bcast(&msg, 1, MPI_INT, 0, newComm);

  printf("Rank %d, color %d, new rank %d has Message: %d\n", rank, color, new_rank, msg);

  MPI_Finalize();

}
