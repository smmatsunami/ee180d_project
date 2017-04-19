#!/bin/bash

DATA_DIR='raw_data'
for i in {0..1000..1}
do
	filename = (echo "data_"$i)
	./imu_data filename
done