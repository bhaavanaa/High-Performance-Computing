%%cu
#include<stdio.h>
#include<math.h>
#define N 100
#define thread_count 5
__global__ void array_add(double *d_result, double *d_a,int n, int thread_cnt)
{
    int load=n/thread_count;
   __shared__ double s_sum[thread_count];
  s_sum[threadIdx.x]=0;
 
    if(threadIdx.x != thread_cnt-1)
    {
        for(int i = (threadIdx.x)*load;i<((threadIdx.x+1)*load); i++)
        {
            s_sum[threadIdx.x]+= d_a[i];
        }
       
    }
      
    if(threadIdx.x == thread_cnt-1)
    {
        
       for(int i = (threadIdx.x)*load;i<n; i++)
        {
            s_sum[threadIdx.x]+= d_a[i];
        }
    }
    __syncthreads();
    
     if(threadIdx.x == 0 )
    {
        *d_result = 0;
        for(long int i=0; i<thread_cnt; i++)
        {
            *d_result += s_sum[i];
        }
    }
  
 
} 
int main()
{
    
    double *a ,out; 
    double *d_a,*d_result;
    
    a   = (double*)malloc(sizeof(double) * N);
    cudaMalloc((void**)&d_a, sizeof(double) * N);
   cudaMalloc((void**)&d_result, sizeof(double));
  
    //out = (double*)malloc(sizeof(double));
    

   

    for(int i = 0; i < N; i++)
    {
        a[i] = i;//pow(2,15)+rand()+0.13246549884;

    }

 cudaMemcpy(d_a, a, sizeof(double)*N,cudaMemcpyHostToDevice);
  
  
  array_add<<<1,thread_count>>>(d_result, d_a,N,thread_count);
 
     cudaMemcpy(&out, d_result, sizeof(double), cudaMemcpyDeviceToHost);
      //for(int i = 0; i < N; i++)
       printf(" sum=   %lf \n",out);
   
}
