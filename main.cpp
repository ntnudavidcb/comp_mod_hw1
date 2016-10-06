#include <stdio.h>              
#include <stdlib.h>             
#include <iostream>


//TODO 
//Have to add in the different statistics for each of the variables
//Use 
//After this a solution on how to bring forth alot of raw data to plot
//Then create graphs from the analysis approach


using namespace std;

#define SIM_END_T 10000 //Run time here, have to see how much to put
#define MEAN_ARR_T 2 //Mean arrival time
#define MEAN_SERV_T 1 //Mean server time, cant go under 1 for some reason


//Random functions taken from lecture notes, ranf gives a random number between 0 and 1, uniform distribution
//expntl return a value from the exponential distribution with mean x
double ranf() // return random number from uniform(0,1)
{
	return (double)rand() / (double)RAND_MAX;
}
double expntl(double x)
{ /* 'expntl' returns a psuedo-random variate from a negative */
  /* exponential distribution with mean x. */
	return(-x*log(ranf()));
}


int main() {

	//Add variables here to keep track of all the values that we have to use
	bool idle_server = true;
	double idle_server_t = 0;
	int queue_size = 0;
	double sim_time = 0;
	double next_serv_time;
	double next_arr_time;
	int num_jobs = 0;
	double queuing_delay = 0;
	double serv_time_tot = next_serv_time = expntl(MEAN_SERV_T);
	double arr_time = next_arr_time = sim_time = expntl(MEAN_ARR_T);

	while (sim_time < SIM_END_T)
	{
		if (sim_time < serv_time_tot) {
			//Put arrival event code here
			if (queue_size == 0 && !idle_server) {
				queue_size = queue_size + 1;
			}
			else if (queue_size == 0 && idle_server) {
				idle_server = false;
				idle_server_t = idle_server_t + next_arr_time;
				next_serv_time = expntl(MEAN_SERV_T);
				serv_time_tot = serv_time_tot + next_serv_time;
			}
			else {
				queue_size = queue_size + 1;
			}
			next_arr_time = expntl(MEAN_ARR_T);
			sim_time = sim_time + next_arr_time;
		}
		else
		{
			//Put departure event code here
			if (queue_size > 0) {
				queue_size = queue_size - 1;
				if (queue_size == 0) {
					idle_server = true;
				}
			}
			else {
				idle_server_t = idle_server_t + next_serv_time;
			}
			next_serv_time = expntl(MEAN_SERV_T);
			serv_time_tot = serv_time_tot + next_serv_time;
		}
	}

	cout << "queue_size: " << queue_size << endl;
	cout << "idle_server: " << idle_server_t << endl;

	return 0;
}