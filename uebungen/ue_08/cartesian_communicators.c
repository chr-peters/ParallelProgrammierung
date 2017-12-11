#include<mpi.h>
#include<stdlib.h>

int main (int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size!=16) {
    if (rank == 0) {
      printf("Error: Number of processes is not equal to 16!\n");
    }
    MPI_Abort(MPI_COMM_WORLD, 0);
  }

  MPI_Comm cart;
  int dims [] = {4, 4};
  int periods [] = {0, 0};
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart);

  // get the new rank
  int rank_new;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_new);

  // get the coordinates
  int coords [2];
  MPI_Cart_coords(cart, rank_new, 2, coords);

  // get the neighbors
  int neighbors [4];
  MPI_Cart_shift(cart, 0, 1, neighbors, neighbors+1);
  MPI_Cart_shift(cart, 1, 1, neighbors+2, neighbors+3);

  // exchange the ranks
  MPI_Request send_requests[4];
  MPI_Request receive_requests[4];
  int receivebuf[4];
  int sendbuf [4] = {rank_new, rank_new, rank_new, rank_new};
  int i;
  for (i=0; i<4; i++) {
    if (neighbors[i] != MPI_PROC_NULL){
      MPI_Isend(sendbuf+i, 1, MPI_INT, neighbors[i], 0, cart, send_requests+i);
      MPI_Irecv(receivebuf+i, 1, MPI_INT, neighbors[i], 0, cart, receive_requests+i);
    } else {
      receivebuf[i] = -1;
    }
  }

  // now wait for the receiving to complete
  MPI_Status receive_states[4];
  for (i=0; i<4; i++) {
    if (neighbors[i] != MPI_PROC_NULL) {
      MPI_Wait(receive_requests+i, receive_states+i);
    }
  }

  printf("rank = %d, rank_cart = %d, coords = %d %d, neighbors(u, d, l, r) = %d %d %d %d, inbuf(u, d, l, r) = %d, %d, %d, %d\n",
	 rank, rank_new, coords[0], coords[1], neighbors[0], neighbors[1],
	 neighbors[2], neighbors[3], receivebuf[0], receivebuf[1], receivebuf[2], receivebuf[3]);

  // now wait for the sending to complete
  MPI_Status send_states[4];
  for (i=0; i<4; i++) {
    if (neighbors[i] != MPI_PROC_NULL) {
      MPI_Wait(send_requests+i, send_states+i);
    }
  }

  MPI_Finalize();
}
