#include <stdio.h>              
#include <stdlib.h>        
#include <math.h>     


//TODO 
//Have to add in the different statistics for each of the variables
//Use 
//After this a solution on how to bring forth alot of raw data to plot
//Then create graphs from the analysis approach

#define SIM_END_T 10000000
//#define MEAN_ARR_T 2 //Mean arrival time
//#define MEAN_SERV_T 1.9 //Mean server time, cant go under 1 for some reason


struct Job{
	double arriveInQueue;
	struct Job* nextPointer;
	struct Job* prevPointer;
};

struct Job* tail;
struct Job* head;

struct Job* getNewJob(){
	struct Job* newJob = (struct Job*)malloc(sizeof(struct Job));
	newJob->nextPointer = NULL;
	newJob->prevPointer = NULL;
	return newJob;
}

void insertJob(double sim_t){
	struct Job* newJob = getNewJob();
	newJob->arriveInQueue = sim_t;
	if (tail == NULL){
		tail = newJob;
		head = newJob;
		return;
	}
	tail->nextPointer = newJob;
	newJob->prevPointer = tail;
	tail = newJob;
}

double deleteJob(double sim_t){ //Returns the time it has waited in the queue
	struct Job* temp;
	temp = head;
	double t = head->arriveInQueue;
	if (head->nextPointer == NULL){
		free(head);
		tail = NULL;
		head = NULL;
		return;
	}
	head = head->nextPointer;
	head->prevPointer = NULL;
	free(temp);
	if (sim_t - t <0)
	{
		printf("%f, %f\n", sim_t, t);
		exit(0);
	}
	return sim_t - t;
}

void deleteLinkedList(){
	while(head){
		struct Job* temp = head;
		free(head);
		head = head->nextPointer;
	}
	tail = NULL;
}
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

// Add parameters to MEAN_ARR_T and MEAN_SERV_T
double* simulation(double MEAN_ARR_T, double MEAN_SERV_T){
	double iterations = 0;
	double accumelated_jobs = 0;
	double accumelated_queue = 0;
	double total_wait_time_queue = 0;
	double total_jobs = 0;
	double idle_server = 1;
	double idle_server_t = 0;
	double queue_size = 0;
	double sim_time = 0;
	double next_serv_time;
	double next_arr_time;
	double serv_time_tot = next_serv_time = expntl(MEAN_SERV_T);
	double arr_time = next_arr_time = sim_time = expntl(MEAN_ARR_T);
	double current_time = 0;

	while (sim_time < SIM_END_T)
	{
		if (sim_time < serv_time_tot) {
			//Put arrival event code here
			current_time = sim_time;
			if (queue_size == 0 && !idle_server) {
				queue_size = queue_size + 1;
				insertJob(current_time);
			}
			else if (queue_size == 0 && idle_server) {
				idle_server = 0;
				idle_server_t = idle_server_t + next_arr_time;
				next_serv_time = expntl(MEAN_SERV_T);
				serv_time_tot = serv_time_tot + next_serv_time;
			}
			else {
				queue_size = queue_size + 1;
				insertJob(current_time);
			}
			total_jobs += 1;

			next_arr_time = expntl(MEAN_ARR_T);
			sim_time = sim_time + next_arr_time;
		}
		else
		{
			//Put departure event code here
			current_time = serv_time_tot;
			if (queue_size > 0) {
				queue_size = queue_size - 1;
				total_wait_time_queue += deleteJob(current_time);
				if (queue_size == 0) {
					idle_server = 1;
				}
			}
			else {
				idle_server_t = idle_server_t + next_serv_time;
			}
			next_serv_time = expntl(MEAN_SERV_T);
			serv_time_tot = serv_time_tot + next_serv_time;
		}
		accumelated_queue += queue_size;
		accumelated_jobs += queue_size + (!idle_server);
		iterations += 1;
	}


	double* results = malloc(3*sizeof(double));
	results[0] = accumelated_jobs / iterations;
	results[1] = accumelated_queue / iterations;
	results[2] = total_wait_time_queue / total_jobs;
	deleteLinkedList();
	return results;		
}

int main() {
	double* results;
	FILE *f1 = fopen("fileJobs100.txt", "w");
	if (f1 == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	FILE *f2 = fopen("fileQueue100.txt", "w");
	if (f2 == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	FILE *f3 = fopen("fileQueueDelay100.txt", "w");
	if (f3 == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}


	//double i;
	//for (i = 1050; i < 100000; i = i + 1)
	//{
		results = simulation(2, 3);	
		printf("average jobs: %f\n", results[0]);
		printf("average queue: %f\n", results[1]);
		printf("average queue delay: %f\n", results[2]);
		fprintf(f1, "%f %f\n",results[0], 1.0);
		fprintf(f2, "%f %f\n",results[1], 1.0);
		fprintf(f3, "%f %f\n",results[2], 1.0);
	//}

	fclose(f1);
	fclose(f2);
	fclose(f3);

	
	return 0;
}