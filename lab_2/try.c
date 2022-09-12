// OpenMP: 2) Matrix multiplication column major order


#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


int main(char argc, char argv[])
{
	double start_time, end_time;

	int omp_rank, omp_threads, N = 500;		

	int fib[20];
	fib[ 0 ] = 1;
	fib[ 1 ] = 1;

#pragma omp parallel for

	for (int i=2; i<20; i++ )
	{
		fib[i] = fib[i-1] + fib[i-2];
	

	}
	printf( "%d\n" ,fib[19] )	;			

															// end of parallelization

	

	return(0);
}