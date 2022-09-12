%%cu
#include <stdio.h>
#include<sys/time.h>
#define M 10
#define N 10
#define num_threads 4


__global__ void add(double *a, double *b, double *c) 
{
   int B=5;
    int elementsReceived = N/num_threads;
    int mini,minik;
    double r;

    if(num_threads != 1)
    {
        int start_index,end_index;
        start_index=(threadIdx.x * elementsReceived);
        if(threadIdx.x==num_threads-1)
          end_index=N;//Num of rows
        else
          end_index=(threadIdx.x + 1)*elementsReceived;
          
        for(int jj=0; jj<N; jj=jj+B)
        for(int kk=0; kk<N; kk=kk+B)
        for(int i=start_index; i<end_index; i++)
        {
             if (jj+B<N)
                mini=jj+B;
            else
                mini=N;
            for(int j=jj;j<mini;j++)
            {
                r=0;
                 if (kk+B<N)
                   minik=kk+B;
                else
                   minik=N;
                for(int k=kk;k<minik;k++)
                  r+=a[i*N+k]* b[k*N+j];   
                
                c[i*N+j]+=r;   
              
            }
            
        }
    }
    else
    {
        for(int jj=0; jj<N; jj=jj+B)
        for(int kk=0; kk<N; kk=kk+B)
        for(int i=0; i<N; i++)
        {
            if (jj+B<N)
                mini=jj+B;
            else
                mini=N;
            for(int j=jj;j<mini;j++)
            {
                r=0;
                  if (kk+B<N)
                   minik=kk+B;
                else
                   minik=N;
             
                for(int k=kk;k<minik;k++)
                    r+=a[i*N+k]* b[k*N+j];
                
                c[i*N+j]+=r;     
            }
        }
    }
  
}

int main(void) {
  double *a, *b, *c;   // host copies of a, b, c 
  double *d_a, *d_b, *d_c;  // device copies of a, b, c 
  int size = sizeof(double)*N*N;
 
  double start,end;
  
  // Allocate space for device copies of a, b, c
  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);

  a=(double *)malloc(size);
  b=(double *)malloc(size);
  c=(double *)malloc(size);

  // Setup input values
  for(int i = 0; i < N; i++)
  {
      for(int j=0;j<N;j++)
      {
         a[i+j*N] = 1;//pow(2,15)+rand()+0.13246549884;;
         b[i+j*N] = 1;//pow(2,16)+rand()+(0.06277549884*j); 
      }   
  }
 
  // Copy inputs to device
  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice); 
  cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice); 

  // Launch add() kernel on GPU
  add<<<1,num_threads>>>(d_a, d_b, d_c);

  // Copy result back to host
  cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
for(int i = 0; i < N; i++)
      for(int j=0;j<N;j++)
             printf("%lf   \n",c[i*N+j]);
  
  // Cleanup
  free(a); free(b); free(c);
  cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
 // printf("%lf",end-start);
  return 0;
 } 
