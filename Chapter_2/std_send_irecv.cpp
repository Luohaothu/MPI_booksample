//std_send_irecv.cpp
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define buf_size 10

int main(int argc, char *argv[])
{
	int myid, numprocs, other;
	int sb[buf_size];
	int rb[buf_size];
	MPI_Status status0, status1;
	MPI_Request req0, req1;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	for (int i = 0; i < buf_size; i++)
	{
		sb[i] = myid + 1;
	}
	if (myid == 0)
	{
		other = 1;
	}
	if (myid == 1)
	{
		other = 0;
	}
	if (myid == 0)
	{
		fprintf(stdout, "Process : %d of %f before sending...\n", myid, MPI_Wtime());
		MPI_Send(sb, buf_size, MPI_INT, other, 1, MPI_COMM_WORLD);
		fprintf(stdout, "Process : %d of %f after sending...\n", myid, MPI_Wtime());

		fprintf(stdout, "Process : %d of %f before ireceiving...\n", myid, MPI_Wtime());
		MPI_Irecv(rb, buf_size, MPI_INT, other, 1, MPI_COMM_WORLD, &req0);
		fprintf(stdout, "Process : %d of %f after ireceiving...\n", myid, MPI_Wtime());

		fprintf(stdout, "Process : %d of %f before waiting...\n", myid, MPI_Wtime());
		MPI_Wait(&req0, &status0);
		fprintf(stdout, "Process : %d of %f after waiting...\n", myid, MPI_Wtime());
	}
	if (myid == 1)
	{
		fprintf(stdout, "Process : %d of %f before ireceiving...\n", myid, MPI_Wtime());
		MPI_Irecv(rb, buf_size, MPI_INT, other, 1, MPI_COMM_WORLD, &req1);
		fprintf(stdout, "Process : %d of %f after ireceiving...\n", myid, MPI_Wtime());

		fprintf(stdout, "Process : %d of %f before sending...\n", myid, MPI_Wtime());
		MPI_Send(sb, buf_size, MPI_INT, other, 1, MPI_COMM_WORLD);
		fprintf(stdout, "Process : %d of %f after sending...\n", myid, MPI_Wtime());

		fprintf(stdout, "Process : %d of %f before waiting...\n", myid, MPI_Wtime());
		MPI_Wait(&req1, &status1);
		fprintf(stdout, "Process : %d of %f after waiting...\n", myid, MPI_Wtime());
	}

	fprintf(stdout, "Hello World! Process %d of %d\n", myid, numprocs);
	MPI_Finalize();
	return 0;
}