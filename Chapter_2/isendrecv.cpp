//isendrecv.cpp
#include "mpi.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
	int myid, numprocs, left, right, buf[10], buf2[10];
	MPI_Request req;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	right = (myid + 1) % numprocs;
	left = myid - 1;
	if (left < 0)
	{
		left = numprocs - 1;
	}
	for (int i = 0; i < 10; i++)
	{
		buf2[i] = myid * numprocs;
	}

	fprintf(stdout, "Process: %d, before isending... at time %f\n", myid, MPI_Wtime());
	MPI_Isend(buf2, 10, MPI_INT, right, 123, MPI_COMM_WORLD, &req);
	fprintf(stdout, "Process: %d, after isending... at time %f\n", myid, MPI_Wtime());

	fprintf(stdout, "Process: %d, before receiving... at time %f\n", myid, MPI_Wtime());
	MPI_Recv(buf, 10, MPI_INT, left, 123, MPI_COMM_WORLD, &status);
	fprintf(stdout, "Process: %d, after receiving... at time %f\n", myid, MPI_Wtime());

	MPI_Wait(&req, &status);
	fprintf(stdout, "Process: %d, isend and irecv finished\n", myid);
	MPI_Finalize();
	return 0;
}
