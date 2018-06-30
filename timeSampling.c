#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void writeTableToFile(char* filename,double* data, int counter);

int main(int argc, char** argv){

	int samples, time_interval;
	samples = atoi(argv[1]);
	time_interval = atoi(argv[2]);
	double* timestamps = malloc(sizeof(double) * samples);
	double* timedif = malloc (sizeof(double) * (samples - 1));
	double min_dif, max_dif, average_dif, standard_deviation, median;
	
	int i;
	struct timeval wtime;
	
	//Collect timestamps
	for (i = 0; i < samples; i++){
		gettimeofday(&wtime, NULL);
		timestamps[i] = (double)((wtime.tv_usec)/1.0e6 + wtime.tv_sec);
		sleep(time_interval);
		printf("Time is %f\n",timestamps[i]);
	}
	
	int counter = 0;
	double temp;
	//Calculate statistics
	for (i = 0; i < samples - 1; i++){
		//Calculate the time differences between each timestamp
		temp = timestamps[i+1] - timestamps[i];
		if (temp < 0)
			continue;
		timedif[counter] = temp;
		//Find min and max of time difference between timestamps
		if(i == 0){
			min_dif = timedif[counter];
			max_dif = timedif[counter];
		}
		else{
			max_dif = (max_dif < timedif[counter]) ? timedif[counter] : max_dif;
			min_dif = (min_dif > timedif[counter]) ? timedif[counter] : min_dif;
		}
		//Calculate the sum for the time difference mean
		average_dif += timedif[counter];
		counter++;
	}
	//Divide by the sample population to get mean
	average_dif = average_dif / counter;
	
	//Claculate standard deviation
	for (i = 0; i < counter; i++){
		standard_deviation += pow(timedif[i] - average_dif, 2);
	}
	standard_deviation = standard_deviation / (counter - 1);
	standard_deviation = sqrt(standard_deviation);
	
	//Calculate median
	median = (counter % 2 == 1) ? timedif[counter / 2] : (timedif[counter / 2]+timedif[counter / 2 + 1])/2;
	
	printf("Average difference is: %f\n", average_dif);
	printf("Standard deviation is: %f\n", standard_deviation);
	printf("Median is: %f\n", median);
	printf("Max is: %f\n", max_dif);
	printf("Min is: %f\n", min_dif);
	
	//Pass data to txt file
	writeTableToFile("timestamps.txt", timestamps, samples);
	writeTableToFile("timedif.txt", timedif, counter);
	
	FILE *f = fopen("statistics.txt", "w");
	if (f == NULL){
		printf("Error opening statistics.txt!\n");
		exit(1);
	}
	fprintf(f,"%.10f\n", average_dif);
	fprintf(f,"%.10f\n", standard_deviation);
	fprintf(f,"%.10f\n", median);
	fprintf(f,"%.10f\n", max_dif);
	fprintf(f,"%.10f\n", min_dif);
	
	fclose(f);
	
}

void writeTableToFile(char* filename,double* data, int counter){
	FILE *f = fopen(filename, "w");
	
	if (f == NULL){
		printf("Error opening %s!\n",filename);
		exit(1);
	}
	int i;
	for (i = 0; i < counter; i++){
		fprintf(f,"%.10f\n", data[i]);
	}
	fclose(f);
}

