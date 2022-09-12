#include<stdio.h>
#include<mpi.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#define size 100000
#define max 9007199254740992

double drand()
{
	double low = 140737488355328;			//2^47
	double high = 281474976710656;			//2^48
	srand((unsigned int)clock());
	return((double)rand() * (high - low)) / (double)max + low;
}

int main(int argc, char **argv)
{
	double t1, t2;

	int procid, numprocs;				
	
	long int s, s0, startIndex, endIndex, i;
	double a[size], sum, part_sum;			

	MPI_Init(&argc, &argv);				
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 	
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);		
	if(procid == 0)
	{
		for(i=0;i<size;i++)
	    {
	    // a[i]= i;
	    	a[i] = rand();
	    // printf("a%d = %lf\n", i, a[i]);   
	    }
 
        s = (int) (size/numprocs);	
		s0 = s + size%numprocs;		
		// printf("s=%d, s0=%d\n", s, s0);
        t1 = MPI_Wtime();	
	}

	MPI_Bcast(&s, 1, MPI_LONG, 0, MPI_COMM_WORLD);	
	MPI_Bcast(&s0, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(a, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	startIndex = s0 + (procid - 1) *s;	
	endIndex = startIndex + s;
	
	sum = 0;
	part_sum = 0;

	if (procid == 0)			
	{
		for(i=0; i < s0; i++)	
		{
			part_sum = part_sum + a[i];
		}
		// printf("Partial Sum is (master) = %lf \n", part_sum);
	}

	else						
	{
		
		for(i=startIndex; i<endIndex; i++)
		{
			part_sum = part_sum + a[i];
			//printf("i-= %d \n", i);
		}
		// printf("Partial Sum  is = %lf from %d\n", part_sum, procid);
	}
	
	MPI_Reduce(&part_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);	// reduction

	if(procid==0) 
	{
		t2 = MPI_Wtime();			
		// printf("The sum of the array is %f \n", sum);			
		printf("time taken is = %f\n", t2-t1);	
	}

	MPI_Finalize();			
}