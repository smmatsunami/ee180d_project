CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SOURCES=${SRC}/math_func.c ${SRC}/main.c ${SRC}/test_neural_network.c ${SRC}/train_neural_net.c ${SRC}/imu_data.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c
EXECUTABLES=$(SOURCES:.c=)

imu_data: producer.c LSM9DS0.c
	$(CC) $(CFLAGS) -o producer producer.c LSM9DS0.c $(LDFLAGS)

clean: 
	rm -f producer