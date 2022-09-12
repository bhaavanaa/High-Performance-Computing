%%cu 
#include<stdio.h>
__global__ void hello()
{
    printf("Hello World from GPU!\n");
}

int main() {
    hello<<<1,2>>>();
        printf("Hello World from GPU!\n");

    return 0;
}
