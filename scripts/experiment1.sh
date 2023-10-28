#!/bin/bash

# This script will run each program with the same arguments (including thread count).
# A CSV file is generated containing a "trial number" index, the name of the program,
# and the runtime reported by the program. This runtime is only for the actual
# algorithm runtime (see the code for more details) and is measured in seconds.

output_file="experiment1_results.csv"
threads=8
programs=(
    "superego-13.2.0"
    "superego-13.2.0-avx"
    "superego-6.5.0"
    "superego-gcc-improve1"
    "superego-gcc-improve1-avx"
    "superego-gcc-improve3"
    "superego-gcc-improve3-avx"
    "superego-gcc-improve4-avx"
    "superego-icc"
    "superego-icc-improve1"
    "superego-icc-improve1-avx"
    "superego-icc-improve3"
    "superego-icc-improve3-avx"
    "superego-icc-improve4"
    "superego-icc-improve4-avx"
)

# Go to the parent folder where the "bin" and "results" folders are located.
cd ../

# Make sure the results folder exists and populate the CSV header for this file.
mkdir -p results
echo "Trial,Compiler,Runtime" > ./results/$output_file

for trial in {1..15}; do
    for program in ${programs[@]}; do
        # Give the user a progress update.
        echo "Trial: $trial, Program: $program"

        # Run the program, capture the output, extract the runtime using grep,
        # and record the output to the CSV file.
        output=`./bin/$program 0.1 3000 3000 0 $threads`
        runtime=`echo $output | grep -oE "Done with threads in [.0-9]+ secs" | grep -oE "[.0-9]+"`
        echo "$trial,$program,$runtime" >> ./results/$output_file
    done
done
