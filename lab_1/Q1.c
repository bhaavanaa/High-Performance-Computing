// OpenMP: 1) Hello World Program. Run experiment for Threads = {1,2,4,6,8,10,12,14,16,20,24}. Estimate the parallelization fraction. Document the report and submit.


#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


int main(char argc, char argv[])
{
	double start_time, end_time;

	int omp_rank, omp_threads;	

	start_time = omp_get_wtime();					

	#pragma omp parallel private(omp_rank)					// beginning of parallelization
	{
		omp_rank = omp_get_thread_num();
		omp_threads = omp_get_num_threads();

		printf("Hello World! by thread number %d, of %d number of threads\n", omp_rank, omp_threads);			// printing "hello world!"
	}														// end of parallelization

	end_time = omp_get_wtime();

	printf("\nTime taken = %f\n", end_time-start_time);

	return(0);
}