#include "feature_detector.h"
#include "math_func.h"

// Divide the stride into 4 segmentations
// Extract max min on each segmentation
Feature* extract_feature(double* data, double* time, int* S_i, int n_S)
{
	int i;
	Feature *features = (Feature*) malloc(sizeof(Feature) * n_S-1);
	
	for(i = 0; i< n_S-1; i++){

		// Segmentation
		float step = (float)(S_i[i+1] - S_i[i]);
		int pos0 = S_i[i];
		int pos1 = S_i[i] + (int)(step / 4.0);
		int pos2 = S_i[i] + (int)(step / 2.0);
		int pos3 = S_i[i] + (int)(3.0 * step / 4.0);
		int pos4 = S_i[i+1];

		
		max(data, pos0, pos1, &features[i].seg0.max);
		min(data, pos0, pos1, &features[i].seg0.min);
		//Add seg max/min + period to array
		max(data, pos1, pos2, &features[i].seg1.max);
		min(data, pos1, pos2, &features[i].seg1.min);
		//Add seg max/min + period to array
		max(data, pos2, pos3, &features[i].seg2.max);
		min(data, pos1, pos2, &features[i].seg2.min);
		//Add seg max/min + period to array
		max(data, pos3, pos4, &features[i].seg3.max);
		min(data, pos3, pos4, &features[i].seg3.min);
		//Add seg max/min + period to array
		
		std(data, pos0, pos1, &features[i].seg0.std);
		std(data, pos1, pos2, &features[i].seg1.std);
		std(data, pos2, pos3, &feautres[i].seg2.std);
		std(data, pos3, pos4, &features[i].seg3.std);
	
		mean(data, pos0, pos1, &features[i].seg0.mean);
		mean(data, pos1, pos2, &features[i].seg1.mean);
		mean(data, pos2, pos3, &feautres[i].seg2.mean);
		mean(data, pos3, pos4, &features[i].seg3.mean);


		
		/*
		find_variance(data, pos0, pos1, &features[i].seg0_var);
		find_variance(data, pos1, pos2, &features[i].seg1_var);
		find_variance(data, pos2, pos3, &features[i].seg2_var);
		find_variance(data, pos3, pos4, &features[i].seg3_var);
		*/
	}

	return features;
}




void stairs_feature(double* accel_y, double* accel_x,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, char* ofile_feature_name)
{
	/* Generic variables */
	int i;
	FILE *fp;

	// Feature needed
	Feature* features0;
	Feature* features1;
	double* period;


	/* feature detection section*/
	// compute period
	period = (double*) malloc(sizeof(double)*(n_S - 1));
	for(i = 0; i < n_S-1; i++){
		period[i] = time[S_i[i+1]] - time[S_i[i]];
	}

	//2 indices divided by 4 
	features1 = extract_feature(accel_x, time, S_i, n_S);
	features0 = extract_feature(accel_y, time, S_i, n_S);

	printf("Attempting to write to file \'%s\'.\n", ofile_feature_name);
	fp = fopen(ofile_feature_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_feature_name
		       );
		exit(EXIT_FAILURE); 
	}

	fprintf(fp, "AY_Seg0_Max,AY_Seg0_Min,AY_Seg0_STD,AY_Seg1_Max,AY_Seg1_Min,AY_Seg2_Max,AY_Seg2_Min,AY_Seg3_Max,AY_Seg3_Min,AX_Seg0_Max,AX_Seg0,Min,AX_Seg0_Mean,AX_Seg1_Max,AX_Seg1_Min,AX_Seg1_Mean,AX_Seg2_Max,AX_Seg2_Min,AX_Seg2_Mean,AX_Seg3_Max,AX_Seg3_Min,AX_Seg3_Mean,Period\n");
	for(i = 0; i < n_S-1; i++){
		fprintf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%20.10lf\n",
		 	features0[i].seg0.max, features0[i].seg0.min, features0[i].seg0.std, 
			features0[i].seg1.max, features0[i].seg1.min, features0[i].seg1.std,
			features0[i].seg2.max, features0[i].seg2.min, features0[i].seg2.std,
			features0[i].seg3.max, features0[i].seg3.min, features0[i].seg3.std,
			features1[i].seg0.max, features1[i].seg0.min, features1[i].seg0.mean,
			features1[i].seg1.max, features1[i].seg1.min, features1[i].seg1.mean,
			features1[i].seg2.max, features1[i].seg2.min, features1[i].seg2.mean,
			features1[i].seg3.max, features1[i].seg3.min, features1[i].seg3.mean,
			period[i]);
		//25 features
	}
	fclose(fp);

	return 0;
}
