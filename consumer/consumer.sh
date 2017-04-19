#! /bin/bash

DATA_DIR='producer_data'
OUTPUT_DIR='consumer_results'
for i in {0..1000..1}
do
	raw_data = (echo $DATA_DIR"/data_"$i)
	echo "Consumer starts..."
	#./consumer $raw_data $OUTPUT_DIR/turn_pt.csv $OUTPUT_DIR/turn_st.csv $OUTPUT_DIR/turn_maxmin.csv $OUTPUT_DIR/turn_feature.csv 50.0
done