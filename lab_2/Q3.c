// OpenMP: 3) Matrix multiplication block based approach


#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int min(int a, int b)
{
	if(a < b)
		return(a);
	else
		return(b);
}

int main(char argc, char argv[])
{
	double start_time, end_time;

	int omp_rank, omp_threads, L = 500, M = 500, N = 500, B = 50;						

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
		for (int jj = 0; jj < N; jj = jj + B)							// performing matrix multiplication using block-based approach
		{
			for (int kk = 0; kk < N; kk = kk + B)
			{
				for (int i = 0; i < N; i++)
				{
					for (int j = jj; j < min(jj+B, N); j++)
					{
						double r = 0;
						for (int k = kk; k < min(kk+B, N); k++)
						{
							r = r + matrix_x[i][k]*matrix_y[k][j];
						}
						matrix_z[i][j] = matrix_z[i][j] + r;
					}
				}
			}
		}
	}																	// end of parallelization

	end_time = omp_get_wtime();

	printf("Time taken = %f\n", end_time-start_time);

	return(0);
}