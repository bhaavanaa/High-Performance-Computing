// MPI: 1) Hello World Program. Run experiment for processors = {1,2,4,6, 8,10,12,14,16,20,24}. Estimate the parallelization fraction. Document and report.


#include<stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int nproc, rank;
	float t1, t2;

	t1 = MPI_Wtime();

	MPI_Init(&argc, &argv);							// initialize MPI

	MPI_Comm_size(MPI_COMM_WORLD, &nproc);			// get Comm size
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);			// get rank

	printf("Hello World from process %d\n", rank);
	
	MPI_Finalize();									// finalize

	t2 = MPI_Wtime();

	printf("Time taken = %f\n", t2-t1);

	return(0);
}