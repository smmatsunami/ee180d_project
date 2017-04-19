#ifndef __TRAIN_NEURAL_NET_H__
#define __TRAIN_NEURAL_NET_H__
#include "fann.h"

int train_global(char *train_file);
int train_walk(char *train_file);
/*
int train_turn(char *train_file);
int train_stair(char *train_file);
int train_jump(char *train_file);
int train_run(char *train_file);
*/
#endif