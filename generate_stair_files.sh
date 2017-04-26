#!/bin/bash

DATA_DIR='data_stairs'
OUTPUT_DIR='stairs_results'

echo "Stride detection on stair ascent data files..."
./stride_detector $DATA_DIR/stair_up.csv $OUTPUT_DIR/stair_up_pt.csv $OUTPUT_DIR/stair_up_st.csv 50.0

#./stride_detector $DATA_DIR/stair_up2.csv $OUTPUT_DIR/stair_up2_pt.csv $OUTPUT_DIR/stair_up2_st.csv 50.0

#./stride_detector $DATA_DIR/stair_up3.csv $OUTPUT_DIR/stair_up3_pt.csv $OUTPUT_DIR/stair_up3_st.csv 50.0

echo "Stride detection on stair descent data files..."
./stride_detector $DATA_DIR/stair_down.csv $OUTPUT_DIR/stair_down_pt.csv $OUTPUT_DIR/stair_down_st.csv 50.0

#./stride_detector $DATA_DIR/stair_down2.csv $OUTPUT_DIR/stair_down2_pt.csv $OUTPUT_DIR/stair_down2_st.csv 50.0

#./stride_detector $DATA_DIR/stair_down3.csv $OUTPUT_DIR/stair_down3_pt.csv $OUTPUT_DIR/stair_down3_st.csv 50.0

echo "Feature detection on stair ascent data files..."
./feature_detector $DATA_DIR/stair_up.csv $OUTPUT_DIR/stair_up_st.csv $OUTPUT_DIR/stair_up_maxmin.csv $OUTPUT_DIR/stair_up_feature.csv

#./feature_detector $DATA_DIR/stair_up2.csv $OUTPUT_DIR/stair_up2_st.csv $OUTPUT_DIR/stair_up2_maxmin.csv $OUTPUT_DIR/stair_up2_feature.csv

#./feature_detector $DATA_DIR/stair_up3.csv $OUTPUT_DIR/stair_up3_st.csv $OUTPUT_DIR/stair_up3_maxmin.csv $OUTPUT_DIR/stair_up3_feature.csv

echo "Feature detection on stair descent data files..."
./feature_detector $DATA_DIR/stair_down.csv $OUTPUT_DIR/stair_down_st.csv $OUTPUT_DIR/stair_down_maxmin.csv $OUTPUT_DIR/stair_down_feature.csv

#./feature_detector $DATA_DIR/stair_down2.csv $OUTPUT_DIR/stair_down2_st.csv $OUTPUT_DIR/stair_down2_maxmin.csv $OUTPUT_DIR/stair_down2_feature.csv

#./feature_detector $DATA_DIR/stair_down3.csv $OUTPUT_DIR/stair_down3_st.csv $OUTPUT_DIR/stair_down3_maxmin.csv $OUTPUT_DIR/stair_down3_feature.csv



 #cat $OUTPUT_DIR/stair_up1_feature.csv > $OUTPUT_DIR/stair_up_feature.csv
 #tail -n +2 $OUTPUT_DIR/stair_up2_feature.csv >> $OUTPUT_DIR/stair_up_feature.csv
 #tail -n +2 $OUTPUT_DIR/stair_up3_feature.csv >> $OUTPUT_DIR/stair_up_feature.csv

 #cat $OUTPUT_DIR/stair_down1_feature.csv > $OUTPUT_DIR/stair_down_feature.csv
 #tail -n +2 $OUTPUT_DIR/stair_down2_feature.csv >> $OUTPUT_DIR/stair_down_feature.csv
 #tail -n +2 $OUTPUT_DIR/stair_down3_feature.csv >> $OUTPUT_DIR/stair_down_feature.csv 

 printf "\n"

./feature_detector $DATA_DIR/stair_up_test.csv $OUTPUT_DIR/stair_up_test_st.csv $OUTPUT_DIR/stair_up_test_maxmin.csv $OUTPUT_DIR/stair_up_test_feature.csv
./feature_detector $DATA_DIR/stair_down_test.csv $OUTPUT_DIR/stair_down_test_st.csv $OUTPUT_DIR/stair_down_test_maxmin.csv $OUTPUT_DIR/stair_down_test_feature.csv


printf "\n"

echo "Generate train files..."
./stairs_train_file_generator $OUTPUT_DIR/stair_up_feature.csv $OUTPUT_DIR/stair_down_feature.csv train_file_dir/train_yiran.txt 17 3



echo "Generate test files..."
./stairs_train_file_generator $OUTPUT_DIR/stair_up_test_feature.csv $OUTPUT_DIR/stair_down_test_feature.csv train_file_dir/test_yiran.txt 17 3



#./train_file_generator pt_results/feature3.csv train_file_dir/train3.txt 3 3 4

#./train_file_generator pt_results/test_feature0.csv train_file_dir/test0.txt 1 3 4

#./train_file_generator pt_results/test_feature1.csv train_file_dir/test1.txt 1 3 4

#./train_file_generator pt_results/test_feature2.csv train_file_dir/test2.txt 2 3 4

#./train_file_generator pt_results/test_feature3.csv train_file_dir/test3.txt 3 3 4


