#ifndef __EXE_NEURAL_NETWORK_H__
#define __EXE_NEURAL_NETWORK_H__
#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include "floatfann.h"

#define TURN 0
#define WALK 1
#define STAIR 2
#define JUMP 3
#define RUN 4

#define FAST_WALK 0
#define MED_WALK 1
#define SLOW_WALK 2

int exe_global_neural_network(char *global_feature_file);
int exe_walk_neural_network(char *walk_feature_file);
//int exe_stair_neural_network(char *stair_feature_file);
//int exe_turn_neural_network(char *turn_feature_file);
//int exe_jump_neural_network(char *jump_feature_file);
//int exe_run_neural_network(char *run_feature_file);


#endif