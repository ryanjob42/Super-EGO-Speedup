#!/bin/bash

# This script will run each program, varying the number of threads allowed.
# All other arguments are the same. A CSV file is generated containing
# the number of threads used, a "trial number" index, the name of the program,
# and the runtimme. This runtime is only for the actual algorithm runtime
# (see the code for more details) and is measured in seconds.
# NOTE: this can take a very long time, so it is recommended that onlhy a
# small subset of the programs are tested.

output_file="experiment2_results.csv"
threadCounts=(1 2 4 8 10 12 14 16 18 20 22 24)
programs=(
    "superego-13.2.0-avx"
    "superego-gcc-improve1"
    "superego-icc-improve3-avx"
    "superego-gcc-improve4-avx"
)

# Go to the parent folder where the "bin" and "results" folders are located.
cd ../

# Make sure the results folder exists and populate the CSV header for this file.
mkdir -p results
echo "Trial,Compiler,Runtime" > ./results/$output_file

for threads in ${threadCounts[@]}; do
    for trial in {1..15}; do
        for program in ${programs[@]}; do
            # Give the user a progress update.
            echo "Threads: $threads, Trial: $trial, Program: $program"

            # Run the program, capture the output, extract the runtime using grep,
            # and record the output to the CSV file.
            output=`./bin/$program 0.1 1000 1000 0 $threads`
            runtime=`echo $output | grep -oE "Done with threads in [.0-9]+ secs" | grep -oE "[.0-9]+"`
            echo "$threads,$trial,$program,$runtime" >> ./results/$output_file
        done
    done
done
