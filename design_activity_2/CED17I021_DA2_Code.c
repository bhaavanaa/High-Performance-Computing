// Import the required libraries
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<mpi.h>


// Define the value of pi
#define PI 3.14159265


// Number of sample points
#define N 10000


// Define the struct which will hold the result of the dft
struct DFT_Coefficient 
{
    double real, img;
};


// Initializing the global variables
float samples[N];                                                           // array which contains the sample values
struct DFT_Coefficient dft_val[N];                                          // array of DFT_Coefficient structures which will hold the result of DFT
double k_by_N[N];                                                           // array which contains the k/N value of the sample
double power_spectrum[N];                                                   // array contains the values of the power spectrum
double sum_power_spectrum = 0, sum_freq_power = 0;                          // initializing the sum variables which will be used for finding the normalized freq


// Main function
int main(int argc, char **argv)
{
    double start_time, end_time;                                         	// to keep track of the start and end time of the paralel process

    int procid, numprocs;													// to keep track of the process id, number of processes
    int chunk, leftover, offset;											// to calculate the amount of work to be distributed to the worker processes
    int i;

    srand(time(NULL));

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &procid);									// obtaining the process id
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);								// obtaining the total number of processes

    chunk = N/numprocs;														// to find the amount of work to be given to each worker proceses
    leftover = N%numprocs;													// allot chunk+leftover to the master process

    // for the master process (initilization, work alloted to the master process, combining the results from the worker processes, display the final output)
    if(procid == 0)										
    {
        // printf("The sample points are-\n");
        for(i = 0; i < N; i++)                                         		// assigning random float values between -1 and 1, inorder to generate a random wave
        {
            // samples[i] = ((float)rand()/RAND_MAX)*(float)(2.0) - 1;
            samples[i] = i;
            // printf("%lf, ", samples[i]);
        }
        // printf("\n\n");

        for(i = 0; i < N; i++)                                      		// initializing the real and img values of DFT_Coefficient struct to 0
        {
            dft_val[i].real = 0;
            dft_val[i].img = 0;
        }

        offset = chunk + leftover;											// offset will be alloted to the master process

        MPI_Status status;

        start_time = MPI_Wtime();											// beginning of the parallel section (note the start time)

        // if the number of processes is greater than 1, then the work is distributed equally to all the workers
        if(numprocs > 1)													
        {
            int k, n, i, i1, j;

            // printf("The DFT value of the samples are-\n");
            for(k = 0; k < offset; k++)                                       					// finding the dft value, k/N value, power spectrum value for the samples alloted to the master process
            {
                for(n = 0; n < N; n++)                                        					// dft value 
                {
                    dft_val[k].real += samples[n] * cos((2 * PI * k * n) / N);
                    dft_val[k].img += samples[n] * sin((2 * PI * k * n) / N);
                }

                k_by_N[k] = (double)k/N;                                            			// k/N value

                power_spectrum[k] = dft_val[k].real*dft_val[k].real + dft_val[k].img*dft_val[k].img;        // power spectrum value

                sum_power_spectrum = sum_power_spectrum + power_spectrum[k];                                // sum of power spectrum values of the master process
                sum_freq_power = sum_freq_power + 2*PI*k_by_N[k]*power_spectrum[k];							// sum of freq values of the master process
                // printf("sample %d: dft = ( %lf ) + ( %lf i)\n", k, dft_val[k].real, dft_val[k].img);
            }
            // printf("\n");
            
            for(i = 1; i < numprocs; i++)														// send the works alloted to the worker processes respectively 
            {
                MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);							// send the index from where the work starts for the worker
                MPI_Send(&samples, N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);						// send all the input samples
              	
                offset = offset + chunk;														// initial offset is done by the master process, the worker process is given chunk of work
            }
            
            double dft_real[chunk];									// initializing the values for receiving results from the worker process												
            double dft_img[chunk];
            double k_by_N_chunck[chunk];
            double power_spectrum_chunck[chunk];
            double part_sum_power_spectrum;
            double part_sum_freq_power;

            for(i = 0; i < chunk; i++)                   			// initializing the real and img values to 0
	        {
	            dft_real[i] = 0;
	            dft_img[i] = 0;
	        }

            for(i1 = 1; i1 < numprocs; i1++)						// receiving all the results from the worker processes
            {
                MPI_Recv(&offset, 1, MPI_INT, i1, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&dft_real, chunk, MPI_DOUBLE, i1, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&dft_img, chunk, MPI_DOUBLE, i1, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&k_by_N_chunck, chunk, MPI_DOUBLE, i1, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&power_spectrum_chunck, chunk, MPI_DOUBLE, i1, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&part_sum_power_spectrum, 1, MPI_DOUBLE, i1, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&part_sum_freq_power, 1, MPI_DOUBLE, i1, 2, MPI_COMM_WORLD, &status);

                sum_power_spectrum = sum_power_spectrum + part_sum_power_spectrum;				// sum the resultant sum values obtained from the worker process
                sum_freq_power = sum_freq_power + part_sum_freq_power;
             
                for(i = offset, j = 0; i < offset+chunk; i++, j++)								// assign the result of the worker process to the final result arrays
                {
                    dft_val[i].real = dft_real[j];
                    dft_val[i].img = dft_img[j];
                    k_by_N[i] = k_by_N_chunck[j];
                    power_spectrum[i] = power_spectrum_chunck[j];
                }
            }
        }

        // if the number of processes is only 1, then the master process does all the work
        else
        {
        	int k, n;
            for(k = 0; k < N; k++)                                            							// finding the dft value, k/N value, power spectrum value for the samples 
            {
                for(n = 0; n < N; n++)                                        							// dft value 
                {
                    dft_val[k].real = dft_val[k].real + samples[n] * cos((2 * PI * k * n) / N);
                    dft_val[k].img = dft_val[k].img + samples[n] * sin((2 * PI * k * n) / N);
                }

                k_by_N[k] = (double)k/N;                                            						// k/N value

                power_spectrum[k] = dft_val[k].real*dft_val[k].real + dft_val[k].img*dft_val[k].img;        // power spectrum value
                
                sum_power_spectrum = sum_power_spectrum + power_spectrum[k];                                // sum of power spectrum values
                sum_freq_power = sum_freq_power + 2*PI*k_by_N[k]*power_spectrum[k];                         // sum of freq*power_spectrum values
                // printf("sample %d: dft = ( %lf ) + ( %lf i)\n", k, dft_val[k].real, dft_val[k].img);
            }
            // printf("\n");
        }

        end_time = MPI_Wtime();															// end of the parallel section (note the end time)

        // printf("The k/N values of the samples are-\n");								// display the final output values
        // for(i = 0; i < N; i++)                                                 
        // {
        //     printf("%lf, ", k_by_N[i]);
        // }
        // printf("\n\n");


        // printf("The power spectrum values are-\n");
        // for(i = 0; i < N; i++)                                                     
        // {
        //     printf("%lf, ", power_spectrum[i]);
        // }
        // printf("\n");

        printf("\nThe normalized freq of the random wave : %lf Hz\n\n", sum_freq_power/sum_power_spectrum);

        printf("Time taken = %f\n", end_time-start_time);                
    }

    // worker process (does the work alloted to it by the master process)
    else
    {
        MPI_Status status;
        
        MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);					// receives the beginning index of the process
        MPI_Recv(&samples, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);				// receives all the sample points
        
        double dft_real[chunk];															// initializing the variables for the worker process															
        double dft_img[chunk];
        double k_by_N_chunck[chunk];
        double power_spectrum_chunck[chunk];
        double part_sum_power_spectrum = 0;
        double part_sum_freq_power = 0;
        int i, k, n, p = 0;

        for(i = 0; i < chunk; i++)                                                 		// initializing the real and img values to 0
        {
            dft_real[i] = 0;
            dft_img[i] = 0;
        }

        for(k = offset; k < offset+chunk; k++, p++)                                    	// finding the dft value, k/N value, power spectrum value for the samples alotted to the worker process
        {
            for (n = 0; n < N; n++)                                        				// dft value 
            {
                dft_real[p] = dft_real[p] + samples[n] * cos((2 * PI * k * n) / N);
                dft_img[p] = dft_img[p] + samples[n] * sin((2 * PI * k * n) / N);
            }
            
            k_by_N_chunck[p] = (double)k/N;                                            			// k/N value
            power_spectrum_chunck[p] = dft_real[p]*dft_real[p] + dft_img[p]*dft_img[p];        	// power spectrum value
            
            part_sum_power_spectrum = part_sum_power_spectrum + power_spectrum_chunck[p];                    	// sum of power spectrum values
            part_sum_freq_power = part_sum_freq_power + 2*PI*k_by_N_chunck[p]*power_spectrum_chunck[p];         // sum of freq*power_spectrum values
            // printf("sample %d: dft = ( %lf ) + ( %lf i)\n", k, dft_real[p], dft_img[p]);
        }
        // printf("\n");

        MPI_Send(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);									// send the output values done by the worker to the master
		MPI_Send(&dft_real, chunk, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&dft_img, chunk, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&k_by_N_chunck, chunk, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&power_spectrum_chunck, chunk, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&part_sum_power_spectrum, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&part_sum_freq_power, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();      
   
    return(0);
}