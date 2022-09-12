#include<stdio.h>
#include<mpi.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#define size 100000
#define max 9007199254740992

double drand()
{
    double low = 140737488355328;           //2^47
    double high = 281474976710656;          //2^48
    srand((unsigned int)clock());
    return((double)rand() * (high - low)) / (double)max + low;
}

int main(int argc, char **argv)
{
    double t1, t2;

    int procid, numprocs;

    long int s, s0, startIndex, endIndex, i;
    double a[size], b[size], dot_product, part_dot_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);   
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    
    if(procid == 0)
    {
        for(i=0; i<size; i++)
        {
            // a[i] = i;
            // b[i] = i;
            a[i] = drand();
            b[i] = drand();
            // printf("%lf, %lf\n", a[i], b[i]);
        }
        s = (int) (size/numprocs);
        s0 = s + size%numprocs;
        // printf("s=%d, s0=%d\n", s, s0);
        t1 = MPI_Wtime();
    }

    MPI_Bcast(&s, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&s0, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(a, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    startIndex = s0 + (procid - 1) *s;
    endIndex = startIndex + s;

    dot_product = 0;
    part_dot_sum = 0;

    if (procid == 0)
    {
        for(i=0; i < s0; i++)
        {
            part_dot_sum = part_dot_sum + a[i]*b[i];
        }
        // printf("Partial Sum is (master) = %f \n", part_dot_sum);
    }

    else
    {
        for(i=startIndex; i<endIndex; i++)
        {
            double z=a[i]*b[i];
            part_dot_sum = part_dot_sum + z;
            // printf("i-= %d   %f  %f    %f \n",i, part_dot_sum,a[i],b[i]);
        }
        // printf("Partial Sum  is = %f from %d\n", part_dot_sum, procid);
    }

    MPI_Reduce(&part_dot_sum, &dot_product, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);     // reduction

    if(procid==0)
    {
        t2 = MPI_Wtime();
        // printf("The dot product of 2 vecotrs is %f \n", dot_product);
        printf("time taken is = %f\n", t2-t1);
    }

    MPI_Finalize();
}