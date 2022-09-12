// OpenMP: 2) Vector Dot Product. Identify the challenge. Run the experiment for Threads={1,2,4,6,8,10,12,14,16,20,24}. Estimate the parallelization fraction.
// a) Implement using Reduction


#include<stdio.h>	
#include<stdlib.h>
#include<omp.h>


int main(char argc, char argv[])
{
	double start_time, end_time;

	int omp_rank, omp_threads;						

	double x[100000], y[100000];

	FILE *myFile;
	FILE *myFile1;
	
    myFile = fopen("random.txt", "r");
    myFile1 = fopen("random1.txt", "r");
    
    for (int i = 0; i < 100000; i++)
    {
        fscanf(myFile, "%lf", &x[i]);
        fscanf(myFile1, "%lf", &y[i]);
    }

    double sum = 0;

    start_time = omp_get_wtime();

	#pragma omp parallel private(omp_rank)							// beginning of parallelization
	{
		omp_rank = omp_get_thread_num();
		omp_threads = omp_get_num_threads();
		
		#pragma omp for reduction(+: sum)
		for(int i=0; i<100000; i=i+1)								// performing vector addition
		{
			sum = sum + x[i]*y[i];
		}
	}																// end of parallelization

	end_time = omp_get_wtime();

	printf("Time taken = %f\n", end_time-start_time);

	return(0);
}