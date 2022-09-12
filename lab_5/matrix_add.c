#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include<math.h>
const int m=1000;
const int n=1000;

int main(int argc, char **argv)
{

	int node;
	int world_size;
	double t1, t2;
	srand(time(NULL));
	MPI_Init(&argc,&argv);
	t1 = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &node);

	//printf("Hello World from Node %d\n",node);

	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if(node == 0)
	{
		double a[m][n],b[m][n],c[m][n];
		for(int i=0;i<m;i++)
		{	
			for(int j=0;j<n;j++)
			{
				a[i][j]= pow(2,15)+rand()+0.13246549884;//rand();
				b[i][j]= pow(2,16)+rand()+0.75245496088;//rand();
			}
		//c[i]=a[i]+b[i];
		//printf("%f   %f\n",a[i],b[i]);
		}
		int load_per_proc;

		if(world_size!=1)
			load_per_proc=(m*n)/(world_size-1);
		else
			load_per_proc=m*n;

		double c1[load_per_proc][n];
		//printf("load== %d \n ",load_per_proc);
		MPI_Status status;

		if(world_size>1)
		{
			for(int i=1;i<world_size;i++)
			{
				MPI_Send(&load_per_proc,1,MPI_INT,i,1,MPI_COMM_WORLD);

				MPI_Send(&a,m*n,MPI_DOUBLE,i,0,MPI_COMM_WORLD);

				MPI_Send(&b,m*n,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			}
			int n1,remi,j;
			for(int i1=1;i1<world_size;i1++)
			{
				MPI_Recv(&n1,1,MPI_INT,MPI_ANY_SOURCE,2,MPI_COMM_WORLD,&status);
				MPI_Recv(&c1,load_per_proc,MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
				
				for(int i=(n1-1)*load_per_proc;i<(n1)*load_per_proc;i++)
					for(int j=0;j<n;j++)
						c[i][j]=c1[i][j];
			}
			int rem = (world_size-1)*load_per_proc;
			rem=rem/m;
			for(int ii= rem;ii<m;ii++)
				for(int jj=0;jj<n;jj++)
					c[ii][jj]=a[ii][jj]+b[ii][jj];
			

		}
		else
		{
		//printf("line no 69");
		for(int i=0;i<m;i++)
			for(int j=0;j<n;j++)
				c[i][j]=a[i][j]+b[i][j];

		}
		//for(int i=0;i<100000;i++)
		//      printf("%f \n ",c[i]);

	}
	else            //worker nodes
	{
	//printf(" in here drom %d \n ",node);
		double a[m][n],b[m][n];
		MPI_Status status;
		int load_per_proc;
		MPI_Recv(&load_per_proc,1,MPI_INT,0,1,MPI_COMM_WORLD,&status); // receivingloadprocess

		MPI_Recv(&a,m*n,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);     //receiving a

		MPI_Recv(&b,m*n,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);    // receiving b

		double c[m][n];
		int j,rem;
		MPI_Send(&node,1,MPI_INT,0,2,MPI_COMM_WORLD);

		for(int i=(node-1)*load_per_proc;i<node*load_per_proc;i++)
			for(int j=0;j<n;j++)
				c[i][j]=a[i][j]+b[i][j];

		MPI_Send(&c,load_per_proc,MPI_DOUBLE,0,0,MPI_COMM_WORLD);

	}
	t2 = MPI_Wtime();
	if(node == 0)
		printf(" time taken :: %f \n",t2-t1);

	MPI_Finalize();

	// printf(" time taken :: %f",t2-t1);
	return 0;
}
