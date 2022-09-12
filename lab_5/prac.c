#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	int node;
	
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &node);

	printf("Hello World from Node %d\n",node);
	if(node == 0)
	{
		int a[10][10];
		for(int i=0;i<10;i++)
			for(int j=0;j<10;j++)
				a[i][j]=i;
		MPI_Send(&a,100,MPI_INT,1,2,MPI_COMM_WORLD);
	}
	else
	{
		int a[10][10];
		MPI_Status status;
		MPI_Recv(&a,100,MPI_INT,0,2,MPI_COMM_WORLD,&status);     //receiving a
		for(int i=0;i<10;i++)
			for(int j=0;j<10;j++)
				printf("i,j ==%d \n",a[i][j]);
	}
MPI_Finalize();

return 0;
}
