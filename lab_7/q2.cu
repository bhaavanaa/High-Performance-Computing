%%cu
#include<stdio.h>
#include<math.h>
#define N 1000000
int thread_count = 24;
__global__ void vector_add(double *d_result, double *d_a, double *d_b, int n, int thread_cnt)
{
    int load=n/thread_cnt;
    if(threadIdx.x != thread_cnt-1)
    {
        for(int i = (threadIdx.x)*load;i<((threadIdx.x+1)*load); i++)
        {
            d_result[i] = d_a[i] + d_b[i];
        }
    }
    if(threadIdx.x == thread_cnt-1)
    {
        for(int i = (threadIdx.x)*load;i<n; i++)
        {
            d_result[i] = d_a[i] + d_b[i];
        }
    }
}

int main()
{
    double *a, *b, *out; 
    double *d_a,*d_b,*d_result;

    a   = (double*)malloc(sizeof(double) * N);
    cudaMalloc((void**)&d_a, sizeof(double) * N);
    b   = (double*)malloc(sizeof(double) * N);
    cudaMalloc((void**)&d_b, sizeof(double) * N);
    out = (double*)malloc(sizeof(double) * N);
    cudaMalloc((void**)&d_result, sizeof(double) * N);

   

    for(int i = 0; i < N; i++)
    {
        a[i] = pow(2,15)+rand()+0.13246549884;;
      b[i] = pow(2,16)+rand()+(0.62477549884*i);
    }
 cudaMemcpy(d_a, a, sizeof(double) * N, cudaMemcpyHostToDevice);
   cudaMemcpy(d_b, b, sizeof(double) * N, cudaMemcpyHostToDevice);
  
  vector_add<<<1,thread_count>>>(d_result, d_a, d_b, N,thread_count);
 
     cudaMemcpy(out, d_result, sizeof(double) * N, cudaMemcpyDeviceToHost);
      for(int i = 0; i < N; i++)
          printf(" i= %d    %lf \n",i,out[i]);
}
