#!/bin/bash

DATA_DIR='producer_data'
for i in {0..1000..1}
do
	filename = (echo $DATA_DIR"/data_"$i)
	./consumer filename
done