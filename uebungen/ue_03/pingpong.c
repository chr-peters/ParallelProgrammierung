#include<mpi.h>

#include<stdio.h>
#include<stdlib.h>

#define LENGTH 1000
#define BUFSIZE 2500000

int main (int argc, char** argv) {
  int length = LENGTH;
  int mode = 2;
  if (argc > 1) {
    length = atoi(argv[1]);
  }
  if (argc > 2) {
    mode = atoi(argv[2]);
  }
  
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rounds = 500;
  int i = rounds;

  MPI_Barrier(MPI_COMM_WORLD);
  double start_time = MPI_Wtime();

  while(i > 0) {

    if (rank == 0) {
      // send ping
      char msg [length];
      if (mode == 0) {
	MPI_Send(msg, length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
      }
      if (mode == 1) {
	MPI_Ssend(msg, length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
      }
      if (mode == 2) {
	char *bufptr;
	int bsize;
	MPI_Buffer_attach(malloc(BUFSIZE), BUFSIZE);
	MPI_Bsend(msg, length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
	MPI_Buffer_detach(&bufptr, &bsize);
      }

      // receive pong
      char answer [length];
      MPI_Status status;
      MPI_Recv(answer, length, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);

      //printf("Rank 0 received message %s.\n", answer);
    }
    
    if (rank == 1) {
      // receive ping
      char answer [length];
      MPI_Status status;
      MPI_Recv(answer, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

      // send pong
      char msg [length];
      if (mode == 0) {
	MPI_Send(msg, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
      }
      if (mode == 1) {
	MPI_Ssend(msg, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
      }
      if (mode == 2) {
	char *bufptr;
	int bsize;
	MPI_Buffer_attach(malloc(BUFSIZE), BUFSIZE);
	MPI_Bsend(msg, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	MPI_Buffer_detach(&bufptr, &bsize);
      }

      //printf("Rank 1 received message %s.\n", answer);
    }

    i--;

  }

  if (rank == 0) {
    double end_time = MPI_Wtime();
    double mean_time = (end_time - start_time)*1000000/rounds;
  
    printf("%d bytes %f us, bandwith 0 MB/s\n", length, mean_time);
  }

  MPI_Finalize();
}
