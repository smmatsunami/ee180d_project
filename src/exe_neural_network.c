#include "exe_neural_network.h"

int exe_global_neural_network(char *global_feature_file)
{
    int i;
    int location;

    float max;
    fann_type *calc_out;
    fann_type input[17];
    struct fann *ann;

    FILE *fp;
    fp = fopen(global_feature_file, "r+");
    //There are total three types of movement which means we will have a 3*3 confusion matix
    int conf_matrix[5][5] = {{0,0,0,0,0},
                             {0,0,0,0,0},
                             {0,0,0,0,0},
                             {0,0,0,0,0},
                             {0,0,0,0,0}};
    int answer[5] = {-1,-1,-1,-1,-1};
    int numLines = 0;
    int inN = 0;
    int outN = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rv;
    char* str_answer;
    
    ann = fann_create_from_file("GLOBAL.net");


    /* get number of lines in the file */
    read = getline(&line, &len, fp);
    rv = sscanf(line, "%d\t%d\t%d\n", &numLines, &inN, &outN);
    if (rv != 3) {
        fprintf(stderr,"Error: Failed to read header line\n");
        exit(EXIT_FAILURE);
    }

    /* start reading the data from the file into the data structures */
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        max = -100;
        /* parse the feature data*/
        rv = sscanf(line, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 
            &input[0], &input[1], &input[2],&input[3],&input[4],
            &input[5], &input[6], &input[7],&input[8],&input[9],
            &input[10], &input[11], &input[12],&input[13],&input[14],
            &input[15], &input[16]);
        if (rv != 17) {
            fprintf(stderr,"Error: Failed to read data line\n");
            exit(EXIT_FAILURE);
        }
        /*Caluculate the type predicted by our trained network*/
        calc_out = fann_run(ann, input);
        for (i = 0; i < 5; i++) {
            if (calc_out[i] > max) {
                max = calc_out[i];
                location = i;
            }
        }

        switch(location) {
            case TURN:
                printf("Got Input values -> Movement type is %s\n", "Turning");
                sleep(1);
                //extract sub_feature
                //exe_Sub_network
                break;
            case WALK:
                printf("Got Input values -> Movement type is %s\n", "Walking");
                sleep(1);
                //extract sub_feature
                //exe_Sub_network
                break;
            case STAIR:
                printf("Got Input values -> Movement type is %s\n", "Stairs");
                sleep(1);
                //extract sub_feature
                //exe_Sub_network
                break;
            case JUMP:
                printf("Got Input values -> Movement type is %s\n", "Jumping");
                sleep(1);
                //extract sub_feature
                //exe_Sub_network
                break;
            case RUN:
                printf("Got Input values -> Movement type is %s\n", "Running");
                sleep(1);
                //extract sub_feature
                //exe_Sub_network
                break;
        }
    }

    fclose(fp);
    fann_destroy(ann);
    return 0;
}

int exe_walk_neural_network(char *walk_feature_file)
{
    int i;
    int location;

    float max;
    fann_type *calc_out;
    fann_type input[9];
    struct fann *ann;

    FILE *fp;
    fp = fopen(walk_feature_file, "r+");
    //There are total three types of movement which means we will have a 3*3 confusion matix
    int conf_matrix[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}};
    int answer[3] = {-1,-1,-1};
    int numLines = 0;
    int inN = 0;
    int outN = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rv;
    char* str_answer;
    
    ann = fann_create_from_file("WALK.net");


    /* get number of lines in the file */
    read = getline(&line, &len, fp);
    rv = sscanf(line, "%d\t%d\t%d\n", &numLines, &inN, &outN);
    if (rv != 3) {
        fprintf(stderr,"Error: Failed to read header line\n");
        exit(EXIT_FAILURE);
    }

    /* start reading the data from the file into the data structures */
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        max = -100;
        /* parse the feature data*/
        rv = sscanf(line, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 
            &input[0], &input[1], &input[2],
            &input[3], &input[4], &input[5],
            &input[6], &input[7], &input[8]);
        if (rv != 9) {
            fprintf(stderr,"Error: Failed to read data line\n");
            exit(EXIT_FAILURE);
        }
        /*Caluculate the type predicted by our trained network*/
        calc_out = fann_run(ann, input);
        for (i = 0; i < 3; i++) {
            if (calc_out[i] > max) {
                max = calc_out[i];
                location = i;
            }
        }

        switch(location) {
            case FAST_WALK:
                printf("\t\tGot Input values -> Walking type is %s\n", "Fast walking");
                sleep(1);
                break;
            case MED_WALK:
                printf("\t\tGot Input values -> Walking type is %s\n", "Med walking");
                sleep(1);
                break;
            case SLOW_WALK:
                printf("\t\tGot Input values -> Walking type is %s\n", "Slow walking");
                sleep(1);
                break;
        }
    }

    fclose(fp);
    fann_destroy(ann);
    return 0;
}




