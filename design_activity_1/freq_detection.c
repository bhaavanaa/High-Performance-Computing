// Import the required libraries
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


// Define the value of pi
#define PI 3.14159265


// Define the struct which will hold the result of the dft
struct DFT_Coefficient 
{
    double real, img;
};


// Number of sample points
int N = 100;


// Main function
int main()
{
	float samples[N];                                                       // array which contains the sample values
    struct DFT_Coefficient dft_val[N];                                      // array of DFT_Coefficient structures which will hold the result of DFT
    double k_by_N[N];                                                       // array which contains the k/N value of the sample
    double power_spectrum[N];                                               // array contains the values of the power spectrum

    srand(time(NULL));

    printf("The sample points are-\n");
	for (int i = 0; i < N; i++)                                             // assigning random float values between -1 and 1, inorder to generate a random wave
	{
		samples[i] = ((float)rand()/RAND_MAX)*(float)(2.0) - 1;
		printf("%lf, ", samples[i]);
	}
	printf("\n\n");

    for(int i = 0; i < N; i++)                                              // initializing the real and img values of DFT_Coefficient struct to 0
    {
        dft_val[i].real = 0;
        dft_val[i].img = 0;
    }

    printf("The DFT value of the samples are-\n");
    for (int k = 0; k < N; k++)                                             // finding the dft value and the k/N value for the samples 
    {
        for (int n = 0; n < N; n++) 
        {
            dft_val[k].real += samples[n] * cos((2 * PI * k * n) / N);
            dft_val[k].img += samples[n] * sin((2 * PI * k * n) / N);
        }
        k_by_N[k] = (double)k/N;
        printf("sample %d: dft = ( %lf ) + ( %lf i)\n", k, dft_val[k].real, dft_val[k].img);
    }
    printf("\n");

    printf("The k/N values of the samples are-\n");
    for(int i=0; i<N; i++)                                                 
    {
        printf("%lf, ", k_by_N[i]);
    }
    printf("\n\n");

    printf("The power spectrum values are-\n");
    for(int i=0; i<N; i++)                                                 // finding the power spectrum 
    {
    	power_spectrum[i] = dft_val[i].real*dft_val[i].real + dft_val[i].img*dft_val[i].img;
        printf("%lf, ", power_spectrum[i]);
    }
    printf("\n");

}