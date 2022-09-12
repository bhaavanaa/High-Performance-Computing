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
#define N 10000


float samples[N];                                                           // array which contains the sample values
struct DFT_Coefficient dft_val[N];                                          // array of DFT_Coefficient structures which will hold the result of DFT
double k_by_N[N];                                                           // array which contains the k/N value of the sample
double power_spectrum[N];                                                   // array contains the values of the power spectrum
double sum_power_spectrum = 0, sum_freq_power = 0;                          // initializing the sum variables which will be used for finding the normalized freq



void initialization()
{
	srand(time(NULL));


    // printf("The sample points are-\n");
	for (int i = 0; i < N; i++)                                                 // assigning random float values between -1 and 1, inorder to generate a random wave
	{
		samples[i] = ((float)rand()/RAND_MAX)*(float)(2.0) - 1;
        // samples[i] = i;
		// printf("%lf, ", samples[i]);
	}
	// printf("\n\n");


    for(int i = 0; i < N; i++)                                                 // initializing the real and img values of DFT_Coefficient struct to 0
    {
        dft_val[i].real = 0;
        dft_val[i].img = 0;
    }
}


void finding_dft()
{
	// printf("The DFT value of the samples are-\n");
    for (int k = 0; k < N; k++)                                            // finding the dft value, k/N value, power spectrum value for the samples 
    {
        for (int n = 0; n < N; n++)                                        // dft value 
        {
            dft_val[k].real += samples[n] * cos((2 * PI * k * n) / N);
            dft_val[k].img += samples[n] * sin((2 * PI * k * n) / N);
        }

        k_by_N[k] = (double)k/N;                                            // k/N value

        power_spectrum[k] = dft_val[k].real*dft_val[k].real + dft_val[k].img*dft_val[k].img;        // power spectrum value
        
        sum_power_spectrum = sum_power_spectrum + power_spectrum[k];                                // sum of power spectrum values
        sum_freq_power = sum_freq_power + 2*PI*k_by_N[k]*power_spectrum[k];                         // sum of freq*power_spectrum values
        // printf("sample %d: dft = ( %lf ) + ( %lf i)\n", k, dft_val[k].real, dft_val[k].img);
    }
    // printf("\n");
}




void finding_normalized_freq()
{
	initialization();	
    
	finding_dft();

    // printf("%lf, %lf\n", sum_freq_power, sum_power_spectrum);
    printf("\nThe normalized freq of the random wave : %lf Hz\n\n", sum_freq_power/sum_power_spectrum);
}


// Main function
int main()
{
	finding_normalized_freq();

    return(0);
}