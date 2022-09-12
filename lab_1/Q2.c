// OpenMP: 2) Vector Addition. Use input as a larger double number (64-bit). Run experiment for Threads = {1,2,4,6,8,10,12,14,16,20,24}. Estimate the parallelization fraction. Document the report and submit.  


#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


int main(char argc, char argv[])
{
	double start_time, end_time;

	int omp_rank, omp_threads;						

	double vectx[345000], vecty[345000], vectz[345000];

	FILE *myFile;
	FILE *myFile1;
    myFile = fopen("random.txt", "r");
    myFile1 = fopen("random1.txt", "r");

    for (int i = 0; i < 345000; i++)
    {
        fscanf(myFile, "%lf", &vectx[i]);
        fscanf(myFile1, "%lf", &vecty[i]);
    }

    start_time = omp_get_wtime();

	#pragma omp parallel private(omp_rank)								// beginning of parallelization
	{
		omp_rank = omp_get_thread_num();
		omp_threads = omp_get_num_threads();

		#pragma omp for
		for(int i=0; i<345000; i=i+1)									// performing vector addition
		{
			for(int j=0; j<62609; j++)
			{
				vectz[i] = vectx[i] + vecty[i];
				vectz[i] = vectz[i] - vecty[i];
				vectz[i] = vectz[i] + vectx[i];
				vectz[i] = vectz[i] - vecty[i];
			}
		}
	}																	// end of parallelization

	end_time = omp_get_wtime();

	printf("Time taken = %f\n", end_time-start_time);

	return(0);
}