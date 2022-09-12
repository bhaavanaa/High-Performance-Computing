%%cu
#include <stdio.h>

#define M 1000
#define N 1000
#define num_threads 1

__global__ void add(double *d_a, double *d_b, double *d_c) 
{
    int load = M/num_threads;
    if(num_threads != 1)
    {
        int start_index,end_index;
        start_index=(threadIdx.x * load);
        if(threadIdx.x==num_threads-1)
          end_index=M;
        else
          end_index=(threadIdx.x + 1)*load;
        for(int i=start_index; i<end_index; i++)
        {
            for(int j=0;j<N;j++)
            {
                for(int k=0;k<N;k++)
                {
                  d_c[i+j*M]=d_c[i+j*M]+d_a[i+k*M]* d_b[k+j*N];   
                }
            }
            
        }
    }
    else
    {
        for(int i=0; i<M; i++)
        {
            for(int j=0;j<N;j++)
            {
                for(int k=0;k<N;k++)
                {
                  d_c[i+j*M]=d_c[i+j*M]+d_a[i+k*M]* d_b[k+j*N];   
                }
            }
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
         a[i+j*M] = pow(2,15)+rand()+0.13246549884;;
         b[i+j*M] = pow(2,16)+rand()+(0.06277549884*j); 
      }   
  }
 

  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice); 
  cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice); 


  add<<<1,num_threads>>>(d_a, d_b, d_c);


  cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);


  free(a); free(b); free(c);
  cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
  return 0;
 } 
