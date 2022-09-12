// OpenMP: 1) Matrix addition


#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


int main(char argc, char argv[])
{
	double start_time, end_time;

	int omp_rank, omp_threads;						

	double matrixx[500][500], matrixy[500][500], matrixz[500][500];

	FILE *myFile;
	FILE *myFile1;
    myFile = fopen("random.txt", "r");
    myFile1 = fopen("random1.txt", "r");

    for (int i = 0; i < 500; i++)
    {
    	for (int j = 0; j < 500; j++)
    	{
    		fscanf(myFile, "%lf", &matrixx[i][j]);
        	fscanf(myFile1, "%lf", &matrixy[i][j]);
    	}
    }

    start_time = omp_get_wtime();

	#pragma omp parallel private(omp_rank)								// beginning of parallelization
	{
		omp_rank = omp_get_thread_num();
		omp_threads = omp_get_num_threads();

		#pragma omp for
		for(int i=0; i<500; i++)										// performing matrix addition
		{
			for(int j=0; j<500; j++)
			{
				matrixz[i][j] = matrixx[i][j] + matrixy[i][j];
			}
		}
	}																	// end of parallelization

	end_time = omp_get_wtime();

	printf("Time taken = %f\n", end_time-start_time);

	return(0);
}