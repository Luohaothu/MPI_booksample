//ibsendrecv.cpp
#include <stdio.h>
#include <mpi.h>

#define BUFSIZE 2000
#define DATALEN 2

int main(int argc, char *argv[])
{
	MPI_Status status;
	MPI_Request request;
	int a[DATALEN], b[DATALEN];
	int buf[BUFSIZE], *bptr, b1, rank, size, errs = 0;

	MPI_Init(0, 0);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Buffer_attach(buf, BUFSIZE);
	bptr = buf;
	b1 = BUFSIZE;

	for (int j = 0; j < DATALEN; j++)
	{
		for (int i = 0; i < DATALEN; i++)
		{
			a[i] = (rank + DATALEN * j)*size + i;
		}
		fprintf(stdout, "Process : %d before ibsend...%f\n", rank, MPI_Wtime());
		MPI_Ibsend(a, DATALEN, MPI_INT, 0, 27 + j, MPI_COMM_WORLD, &request);
		fprintf(stdout, "Process : %d after ibsend...%f\n", rank, MPI_Wtime());
		fprintf(stdout, "Process : %d before wait...%f\n", rank, MPI_Wtime());
		MPI_Wait(&request, &status);
		fprintf(stdout, "Process : %d after wait...%f\n", rank, MPI_Wtime());
	}
	if (rank == 0)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < DATALEN; j++)
			{
				int k;
				status.MPI_TAG = -10;
				status.MPI_SOURCE = -20;

				fprintf(stdout, "Process : %d before recv...%f\n", rank, MPI_Wtime());
				MPI_Recv(b, DATALEN, MPI_INT, i, 27 + j, MPI_COMM_WORLD, &status);
				fprintf(stdout, "Process : %d after recv...%f\n", rank, MPI_Wtime());

				if (status.MPI_TAG!=27+j)
				{
					errs++;
					printf("Wrong tag=%d\n", status.MPI_TAG);
					fflush(stdout);
				}
				if (status.MPI_SOURCE!=i)
				{
					errs++;
					printf("Wrong source=%d\n", status.MPI_SOURCE);
					fflush(stdout);
				}
				for (int k = 0; k < DATALEN; k++)
				{
					if (b[k] != (i+DATALEN*j)*size + k)
					{
						errs++;
						printf("received b[%d]=%d from %d tag %d\n", k, b[k], 1, 27 + j);
						fflush(stdout);
					}
				}
			}
		}
	}
	MPI_Buffer_detach(&bptr, &b1);
	MPI_Finalize();
	return errs;
}