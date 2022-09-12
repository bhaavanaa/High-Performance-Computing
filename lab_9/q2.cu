%%cu
#include<stdio.h>
#include<math.h>
#define N 10
#define thread_count 5

__global__ void dot_pro(double *d_result, double *d_a,double *d_b,int n, int thread_cnt)
{
    int load=n/thread_count;
   __shared__ double s_dsum[N];
  //s_dsum[threadIdx.x]=0;
 
    if(threadIdx.x != thread_cnt-1)
    {
        for(int i = (threadIdx.x)*load;i<((threadIdx.x+1)*load); i++)
        {
            s_dsum[i]= d_a[i]*d_b[i];
        }
       
    }
      
    if(threadIdx.x == thread_cnt-1)
    {
        
       for(int i = (threadIdx.x)*load;i<n; i++)
        {
            s_dsum[i]= d_a[i]*d_b[i];
        }
    }
    __syncthreads();
    
     if(threadIdx.x == 0 )
    {
        *d_result = 0;
        for( int i=0; i<N; i++)
        {
            *d_result += s_dsum[i];
        }
    }
  
 
} 
int main()
{
    
    double *a,*b ,out; 
    double *d_a,*d_b,*d_result;
    
    a   = (double*)malloc(sizeof(double) * N);
    b   = (double*)malloc(sizeof(double) * N);
    
    cudaMalloc((void**)&d_a, sizeof(double) * N);
    cudaMalloc((void**)&d_b, sizeof(double) * N);
    cudaMalloc((void**)&d_result, sizeof(double));
  
    //out = (double*)malloc(sizeof(double));
    

   

    for(int i = 0; i < N; i++)
    {
        a[i] = i;//pow(2,15)+rand()+0.13246549884;
        b[i] = i;
    }

    cudaMemcpy(d_a, a, sizeof(double)*N,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(double)*N,cudaMemcpyHostToDevice);
  
  dot_pro<<<1,thread_count>>>(d_result, d_a,d_b,N,thread_count);
 
     cudaMemcpy(&out, d_result, sizeof(double), cudaMemcpyDeviceToHost);
      //for(int i = 0; i < N; i++)
       printf(" sum=   %lf \n",out);
   
}
