//ssendrecv.cpp
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int rank, size;
	int buffer[10];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (size < 2)
	{
		printf("Please run with two processes.\n");
		fflush(stdout);
		MPI_Finalize();
		return 0;
	}
	if (rank == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			buffer[i] = i;
		}
		printf("proc: %d sending...\n", rank);
		MPI_Ssend(buffer, 10, MPI_INT, 1, 123, MPI_COMM_WORLD);
	}
	if (rank == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			buffer[i] = -1;
		}
		printf("proc: %d receving...\n", rank);
		MPI_Recv(buffer, 10, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
		for (int i = 0; i < 10; i++)
		{
			if (buffer[i] != i)
			{
				printf("Error: buffer[%d] = %d, but is expected to be %d\n", i, buffer[i], i);

			}
			else
			{
				printf("proc%d: buffer[%d] = %d is as expected be %d\n", rank, i, buffer[i], i);

			}
			fflush(stdout);
		}
	}
	MPI_Finalize();
	return 0;
}
