#!/bin/bash

# This script will run each program with the same arguments (including thread count).
# Each program will output the matches it found to its own file in the "outputs" folder.
# Once this is done for all programs, a Python script is called to check their validity
# and output the results to a CSV file in the "results" folder.

threads=8
programs=(
    "superego-13.2.0-avx-validate"
    "superego-13.2.0-validate"
    "superego-6.5.0-avx-validate"
    "superego-6.5.0-validate"
    "superego-gcc-improve1-avx-validate"
    "superego-gcc-improve1-validate"
    "superego-gcc-improve3-avx-validate"
    "superego-gcc-improve3-validate"
    "superego-gcc-improve4-avx-validate"
    "superego-icc-improve1-avx-validate"
    "superego-icc-improve1-validate"
    "superego-icc-improve3-avx-validate"
    "superego-icc-improve3-validate"
    "superego-icc-improve4-avx-validate"
    "superego-icc-improve4-validate"
    "superego-icc-validate"
)

# Go to the parent folder where the "bin" and "results" folders are located.
cd ../

# Unzip the data file that's used to test validity.
if [ ! -f ColorHist.txt ]; then
    unzip -o SuperEGO_data.zip ColorHist.txt
fi

# Make sure the "outputs" directory exists and nothing is in it.
mkdir -p outputs
count=`ls -1 outputs/* 2>/dev/null | wc -l`
if [ $count != 0 ]; then
    rm outputs/*
fi

# For some reason, this module needs to be loaded.
module purge
module load gcc/13.2.0

# Run all the programs once to have them record their results.
for program in ${programs[@]}; do
    echo "Running $program"
    ./bin/$program 0.1 68 68 2 8 > /dev/null
done

echo "Running the Python script to check validity."
module purge
module load python/anaconda/py3.10-2023.03
python3 ./scripts/check_validity.py
