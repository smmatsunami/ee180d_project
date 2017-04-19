CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SRC=consumer
SOURCES=${SRC}/math_func.c ${SRC}/main.c ${SRC}/test_neural_network.c ${SRC}/train_neural_net.c ${SRC}/imu_data.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c
EXECUTABLES=$(SOURCES:.c=)

all: consumer

consumer: ${SRC}/main.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c ${SRC}/math_func.c
	$(CC) $(CFLAGS) -o main ${SRC}/main.c ${SRC}/feature_detector.c ${SRC}/stride_detector.c ${SRC}/math_func.c -lm $(LDFLAGS)

clean:
	rm -f consumer
	rm -f *~
	rm -f GLOBAL.net
	rm -f WALK.net
	rm -f test_data.txt