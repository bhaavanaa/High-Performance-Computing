%%cu
#include <stdio.h>
#include<sys/time.h>
#define M 10
#define N 10
#define num_threads 2


__global__ void add(double *d_a, double *d_b, double *d_c) 
{
    int load = M/num_threads;
    if(num_threads != 1)
    {
        int start_index,end_index;
        start_index=(threadIdx.x * load);
        if(threadIdx.x==num_threads-1)
          end_index=start_index+N-start_index;
        else
          end_index=(threadIdx.x + 1)*load;
        for(int i=start_index; i<end_index; i++)
        {
            for(int j=0;j<N;j++)
            {
              d_c[i*N+j]=d_a[i*N+j]+ d_b[i*N+j];  
            }
            
        }
    }
    else
    {
        for(int i=0; i<M; i++)
        {
            for(int j=0;j<N;j++)
            d_c[i*N+j]=d_a[i*N+j]+ d_b[i*N+j];
        }
    }
  
}

int main(void) {
  double *a, *b, *c;   
  double *d_a, *d_b, *d_c;
  int size = sizeof(double)*M*N;

  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);

  a=(double *)malloc(size);
  b=(double *)malloc(size);
  c=(double *)malloc(size);


  for(int i = 0; i < M; i++)
  {
      for(int j=0;j<N;j++)
      {
         a[i*N+j] = pow(2,15)+rand()+0.13246549884;;
         b[i*N+j] = pow(2,16)+rand()+(0.12345678904*j); 
      }
      
  }


  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice); 
  cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice); 


  add<<<1,num_threads>>>(d_a, d_b, d_c);


  cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

   /*for(int i = 0; i < M; i++)
      for(int j=0;j<N;j++)
        printf("%lf",c[i*N+j]);*/

  free(a); free(b); free(c);
  cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);

  return 0;
 } 
