//bsendirecv.cpp
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	MPI_Comm comm = MPI_COMM_WORLD;
	int dest = 1, src = 0, tag = 1;
	int s1, s2, s3;
	char *buf, *bbuf;
	char msg1[7], msg3[17];
	double msg2[2];
	char rmsg1[64], rmsg3[64];
	double rmsg2[64];
	int errs = 0, rank;
	int bufsize, bsize;
	MPI_Status status;
	MPI_Request req;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	MPI_Pack_size(7, MPI_CHAR, comm, &s1);
	MPI_Pack_size(2, MPI_DOUBLE, comm, &s2);
	MPI_Pack_size(17, MPI_CHAR, comm, &s3);

	bufsize = 3 * MPI_BSEND_OVERHEAD + s1 + s2 + s3;
	buf = (char*)malloc(bufsize);
	MPI_Buffer_attach(buf, bufsize);

	strncpy(msg1, "012345", 7);
	strncpy(msg3, "0123401234012345", 17);
	msg2[0] = 1.23;
	msg2[1] = 3.21;
	if (rank == src)
	{
		fprintf(stdout, "-----proc %d, before send, t=%f\n", rank, MPI_Wtime());
		MPI_Bsend(msg1, 7, MPI_CHAR, dest, tag, comm);
		fprintf(stdout, "-----proc %d, after send, t=%f\n", rank, MPI_Wtime());
	}

	if (rank == dest)
	{
		fprintf(stdout, "-----proc %d, before recv, t=%f\n", rank, MPI_Wtime());
		MPI_Irecv(rmsg1, 7, MPI_CHAR, src, tag, comm, &req);
		fprintf(stdout, "-----proc %d, after recv, t=%f\n", rank, MPI_Wtime());

		fprintf(stdout, "-----proc %d, before wait, t=%f\n", rank, MPI_Wtime());
		MPI_Wait(&req, &status);
		fprintf(stdout, "-----proc %d, after wait, t=%f\n", rank, MPI_Wtime());

		if (strcmp(rmsg1, msg1) != 0)
		{
			errs++;
			fprintf(stdout, "message 1 (%s) shoule be %s\n", rmsg1, msg1);
			fflush(stdout);
		}
		else
		{
			fprintf(stdout, "message transform successfully\n");
		}
	}

	fprintf(stdout, "<<<<<<<<<<proc %d , before detach, t=%f\n", rank, MPI_Wtime());
	MPI_Buffer_detach(&buf, &bufsize);
	fprintf(stdout, ">>>>>>>>>>proc %d , after detach, t=%f\n", rank, MPI_Wtime());

	free(buf);
	MPI_Finalize();
	return 0;
}