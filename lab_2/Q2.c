// OpenMP: 2) Matrix multiplication column major order


#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


int main(char argc, char argv[])
{
	double start_time, end_time;

	int omp_rank, omp_threads, N = 500;						

	double matrix_x[500][500], matrix_y[500][500], matrix_z[500][500];

	FILE *myFile;
	FILE *myFile1;
    myFile = fopen("random.txt", "r");
    myFile1 = fopen("random1.txt", "r");

    for (int i = 0; i < 500; i++)
    {
    	for (int j = 0; j < 500; j++)
    	{
    		fscanf(myFile, "%lf", &matrix_x[i][j]);
    		fscanf(myFile1, "%lf", &matrix_y[i][j]);
    		matrix_z[i][j] = 0;
    	}
    }

    start_time = omp_get_wtime();

	#pragma omp parallel private(omp_rank)								// beginning of parallelization
	{
		omp_rank = omp_get_thread_num();
		omp_threads = omp_get_num_threads();

		#pragma omp for
		for (int i = 0; i < N; i++)										// performing matrix multiplication column major order
		{
			for (int j = 0; j < N; j++)
			{
				for (int k = 0; k < N; k++)
				{
					matrix_z[j][k] += matrix_x[j][i] * matrix_y[i][k];
				}
			}
		}
	}																	// end of parallelization

	end_time = omp_get_wtime();

	printf("Time taken = %f\n", end_time-start_time);

	return(0);
}