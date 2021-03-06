/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
/* for fabsf() */
#include <math.h>

#define BUFF_SIZE 1024


struct Data{
	double time;
	double accel_x;
	//double accel_y;
	//float accel_z;
};


/*
 * sets first <n> values in <*arr> to <val>
 */
void clear_buffer(int *arr, int val, int n) 
{
	int i;
	for (i = 0; i < n; i++) {
		arr[i] = val;
	}
}

/*
 * Caculates mean of first <n> samples in <*arr>
 */
/*float calculate_mean(float *arr, int n)
{
	float total;
	int i;

	total = 0.0f;
	for (i = 0; i < n; i++) {
		total += arr[i];
	}

	return total/((float) n);
}*/

int 
find_peaks_and_troughs(
		struct Data *arr, 	// signal 
		int n_samples, 	// number of samples present in the signal
		float E, 	// threshold for peak detection
		// arrays that will store the indicies of the located
		// peaks and troughs
		int *P, int *T,
		// number of peaks (n_P) and number of troughs (n_T)
		// found in the data set *arr
		int *n_P, int *n_T
		)
{
	int a, b, i, d, _n_P, _n_T;

	i = -1; d = 0; a = 0; b = 0;
	_n_P = 0; _n_T = 0;

	clear_buffer(P, 0, n_samples);
	clear_buffer(T, 0, n_samples);

	while (i != n_samples) {
		i++;
		if (d == 0) {
			if (arr[a].accel_x >= (arr[i].accel_x + E)) {
				d = 2;
			} else if (arr[i].accel_x >= (arr[b].accel_x + E)) {
				d = 1;
			}
			if (arr[a].accel_x <= arr[i].accel_x) {
				a = i;
			} else if (arr[i].accel_x <= arr[b].accel_x) {
				b = i;
			}
		} else if (d == 1) {
			if (arr[a].accel_x <= arr[i].accel_x) {
				a = i;
			} else if (arr[a].accel_x >= (arr[i].accel_x + E)) {
				/*
				 * Peak has been detected.
				 * Add index at detected peak
				 * to array of peak indicies
				 * increment count of peak indicies
				 */
				P[_n_P] = a;
				_n_P++;
				b = i;
				d = 2;
			}
		} else if (d == 2) {
			if (arr[i].accel_x <= arr[b].accel_x) {
				b = i;
			} else if (arr[i].accel_x >= (arr[b].accel_x + E)) {
				/*
				 * Trough has been detected.
				 * Add index at detected trough
				 * to array of trough indicies
				 * increment count of trough indicies
				 */
				T[_n_T] = b;
				_n_T++;
				a = i;
				d = 1;
			}
		}
	}

	(*n_P) = _n_P;
	(*n_T) = _n_T;
	return 0;
}



int main(int argc, char **argv)
{
	/* Generic variables */
	int i, idx;
	int rv;
	/* Variables for reading file line by line */
	char *ifile_name, *ofile_pt_name, *ofile_st_name, *ofile_feature_name;
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int N_SAMPLES;

	/* Variables for storing the data and storing the return values */
	float accel_y, accel_z, gyro_x, gyro_y, gyro_z; 	// variables for data collected from input file
	// *t, *accel_x, 
	struct Data *data_arry;
	float pk_threshold;	// pk-threshold value
	float time_constraint;

	double t1, t2; // variable used to parse time_before and time_after
	double start_time; // variable used to computer sampling time
       	/* Variables for peak-trough detection */	
	int *P_i; 	// indicies of each peak found by peak detection
	int *T_i; 	// indicies of each trough found by trough detection
	int *S_i; 	// indicies of the start of each stride
	
	// Feature needed
	int *peak;
	int *trough;
	double* period;

	int n_P; 	// number of peaks
	int n_T; 	// number of troughs
	int n_S; 	// number of strides

	/*
	 * set this to 0 so that the code can function without
	 * having to actually performing stride detection
	 * from peaks and troughs
	 */
	n_S = 0; 
	
	/*
	 * Check if the user entered the correct command line arguments
	 * Usage: 
	 * ./extract_stride_data <ifile_name> <output_peaks> <output_strides>
	 * 				<threshold_value_float>
	 * Or 
	 * ./extract_stride_data
	 */
	if (argc != 7) {
		fprintf(stderr, "USEAGE: ./extract_stride_data <input file> <peak trough detection output file> <stride detection output file> <feature output file> <threshold> <time constraints>");
		exit(EXIT_FAILURE);

	} else {
		ifile_name = argv[1];
		ofile_pt_name = argv[2];
		ofile_st_name = argv[3];
		ofile_feature_name = argv[4];
		pk_threshold = atof(argv[5]);
		time_constraint = atof(argv[6]);
	}

	printf("Arguments used:\n\t%s=%s\n\t%s=%s\n\t%s=%s\n\t%s=%f\n",
			"ifile_name", ifile_name,
			"ofile_peak_trough_name", ofile_pt_name,
			"ofile_stride_name", ofile_st_name,
			"peak_threshold", pk_threshold
	      );

	/* open the input file */
	printf("Attempting to read from file \'%s\'.\n", ifile_name);
	fp = fopen(ifile_name, "r");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to read from file \'%s\'.\n", 
				ifile_name
		       );
		exit(EXIT_FAILURE);
	}

	/* count the number of lines in the file */
	read = getline(&line, &len, fp); //discard header of file
	N_SAMPLES = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		N_SAMPLES++;
	}

	/* go back to the start of the file so that the data can be read */
	rewind(fp);
	read = getline(&line, &len, fp); //discard header of file

	/* start reading the data from the file into the data structures */
	i = 0;

	data_arry = (struct Data *) malloc(sizeof(struct Data) * N_SAMPLES);

	while ((read = getline(&line, &len, fp)) != -1) {
		/* parse the data */
		rv = sscanf(line, "%lf,%lf,%lf,%f,%f,%f,%f,%f\n", &t1, &t2, &(data_arry[i].accel_x), &accel_y, &accel_z, &gyro_x, &gyro_y, &gyro_z);
		if (rv != 8) {
			fprintf(stderr,
					"%s %d \'%s\'. %s.\n",
					"Failed to read line",
					i,
					line,
					"Exiting"
			       );
			exit(EXIT_FAILURE);
		}
		if(i == 0)
			start_time = t1;

		//fprintf(stdout, "%20.10lf, %20.10lf\n", t1, t2);
		data_arry[i].time = (t1 + t2)/2.0 - start_time;
		i++;
	}
	fclose(fp);

	/* 
	 * From selected thresholds, 
	 * find indicies of peaks
	 * find indicies of troughs
	 */
	P_i = (int *) malloc(sizeof(int) * N_SAMPLES);
	T_i = (int *) malloc(sizeof(int) * N_SAMPLES);
	
	rv = find_peaks_and_troughs(
			data_arry, 
			N_SAMPLES, 
			pk_threshold, 
			P_i, T_i, 
			&n_P, &n_T);
	if (rv < 0) {
		fprintf(stderr, "find_peaks_and_troughs failed\n");
		free(P_i);
		free(T_i);
		free(data_arry);
		exit(EXIT_FAILURE);
	}
	P_i = (int *) realloc(P_i, sizeof(int) * n_P);
	T_i = (int *) realloc(T_i, sizeof(int) * n_T);

	/* DO NOT MODIFY ANYTHING BEFORE THIS LINE */

	/* 
	 * Insert your algorithm to convert from a series of peak-trough
	 * indicies, to a series of indicies that indicate the start
	 * of a stride.
	 */


	/* DO NOT MODIFY ANYTHING AFTER THIS LINE */
	printf("Attempting to sort.\n");
	S_i = (int *) malloc(sizeof(int) * (n_P + n_T));
	peak = (int *) malloc(sizeof(int) * (n_P));
	trough = (int*) malloc(sizeof(int) * (n_T));
	int peak_num = 0;
	int trough_num = 0;

	int idx_p;
	int idx_t;
	int j;
	i = 0;
	j = 0;
	while(i < n_P && j < n_T){
		idx_p = P_i[i];
		idx_t = T_i[j];

		// if peak is in front of trough
		if(data_arry[idx_p].time < data_arry[idx_t].time){
			// 1st one is always good
			if(n_S==0){
				S_i[n_S] = P_i[i];
				peak[peak_num] = P_i[i];
				peak_num++;
				i++;
				n_S++;
			}
			// if peak is far enough from previous peak/trough
			else if(data_arry[idx_p].time - data_arry[S_i[n_S - 1]].time >= time_constraint){
				S_i[n_S] = P_i[i];
				peak[peak_num] = P_i[i];
				peak_num++;
				i++;
				n_S++;
			}
			// peak is too close to previous peak/trough, skip
			else
				i++;
		}
		// if trough is in front of trough
		else{
			// 1st one is always good
			if(n_S==0){
				S_i[n_S] = T_i[j];
				trough[trough_num] = T_i[j];
				trough_num++;
				j++;
				n_S++;
			}
			// if the trough is far enough from previous peak/trough
			else if(data_arry[idx_t].time - data_arry[S_i[n_S - 1]].time >= time_constraint){
				S_i[n_S] = T_i[j];
				trough[trough_num] = T_i[j];
				trough_num++;
				j++;
				n_S++;
			}
			// trough is too close to previous peak/trough, skip
			else
				j++;
		}
	}
	while(i < n_P){
		idx_p = P_i[i];
		// if peak is far enough from previous peak/trough, keep
		if(data_arry[idx_p].time - data_arry[S_i[n_S - 1]].time >= time_constraint){
			S_i[n_S] = P_i[i];
			peak[peak_num] = P_i[i];
			peak_num++;
			n_S++;
			i++;
		}
		// too far from prevous peak/trough, skip
		else
			i++;
	}
	while(j < n_T){
		idx_t = T_i[j];
		// if trough is far enough from previous peak/trough, keep
		if(data_arry[idx_t].time - data_arry[S_i[n_S - 1]].time >= time_constraint){
			S_i[n_S] = T_i[j];
			trough[trough_num] = T_i[j];
			trough_num++;
			n_S++;
			j++;
		}
		// too far, skip
		else
			j++;
	}

	period = (double*) malloc(sizeof(double)*(peak_num - 1));
	for(i = 0; i < peak_num-1; i++){
		period[i] = data_arry[peak[i+1]].time - data_arry[peak[i]].time;
	}

	/* open the output file to write the peak and trough data */
	printf("Attempting to write to file \'%s\'.\n", ofile_pt_name);
	fp = fopen(ofile_pt_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_pt_name
		       );
		free(P_i);
		free(T_i);
		free(S_i);
		free(data_arry);
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "P_t,P_x,T_t,T_p\n");
	for (i = 0; i < n_P || i < n_T; i++) {
		/* Only peak data if there is peak data to write */
		if (i < n_P) {
			idx = P_i[i];
			fprintf(fp, "%20.10lf,%lf,",
					data_arry[idx].time,
				data_arry[idx].accel_x);
		} else {
			fprintf(fp, ",,,");
		}
		/* Only trough data if there is trough data to write */
		if (i < n_T) {
			idx = T_i[i];
			fprintf(fp, "%20.10lf,%lf\n",
					data_arry[idx].time,
					data_arry[idx].accel_x);
		} else {
			fprintf(fp, ",,\n");
		}
	}
	fclose(fp);

	/* open the output file to write the stride data */
	printf("Attempting to write to file \'%s\'.\n", ofile_st_name);
	fp = fopen(ofile_st_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_st_name
		       );
		free(P_i);
		free(T_i);
		free(S_i);
		free(data_arry);
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "S_t,S_x\n");
	for (i = 0; i < n_S; i++) {
		idx = S_i[i];
		fprintf(fp, "%20.10lf,%lf\n",
				data_arry[idx].time,
				data_arry[idx].accel_x);
	}
	fclose(fp);

	printf("Attempting to write to file \'%s\'.\n", ofile_feature_name);
	fp = fopen(ofile_feature_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_st_name
		       );
		free(P_i);
		free(T_i);
		free(S_i);
		free(data_arry);
		free(peak);
		free(trough);
		free(period);
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "Stride_Max,Stride_Min,Period\n");
	for(i = 0; i < peak_num-1 && i < trough_num; i++){
		fprintf(fp, "%lf,%lf,%20.10lf\n", data_arry[peak[i]].accel_x, data_arry[trough[i]].accel_x, period[i]);
	}
	fclose(fp);

	free(data_arry);
	free(P_i);
	free(T_i);
	free(S_i);
	free(peak);
	free(trough);
	free(period);

	printf("extract_stride_data completed successfuly. Exiting.\n");

	return 0;
}
