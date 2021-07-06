#!/bin/bash
set -e
make toSketchOMP
for file in images/*.jpg
do
    times_file='times/times_'$(basename "$file" | sed 's/\(.*\)\..*/\1/')'.txt'
    out_file='images_sketch/'$(basename "$file" | sed 's/\(.*\)\..*/\1/')'_sketch.'${file##*.}
    echo "Processing" ${file##*/}"..........."
    echo "" > $times_file

    for num_threads in $@
    do
        echo "--------------------------------------------">> $times_file
        echo "TIMES FOR" ${file##*/} "WITH" $num_threads "THREADS" >> $times_file
        echo "--------------------------------------------">> $times_file
        echo "" >> $times_file
        for i in {1..10}
        do
            ./toSketchOMP $file $out_file $num_threads >> $times_file
        done
        echo "" >> $times_file
    done
done

./graphing.py
echo "Finished :)"