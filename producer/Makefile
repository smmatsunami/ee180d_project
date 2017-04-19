CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SRC=src
SOURCES=${SRC}/math_func.c ${SRC}/main.c ${SRC}/test_neural_network.c ${SRC}/train_neural_net.c ${SRC}/imu_data.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c
EXECUTABLES=$(SOURCES:.c=)

imu_data: ${SRC}/imu_data.c ${SRC}/LSM9DS0.c
	$(CC) $(CFLAGS) -o imu_data ${SRC}/imu_data.c ${SRC}/LSM9DS0.c $(LDFLAGS)

clean: 
	rm -f imu_data