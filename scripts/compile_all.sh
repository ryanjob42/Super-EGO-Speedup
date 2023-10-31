#!/bin/bash

#####################################################################
# Notes
#####################################################################

# This script is intended to be run on Computer Science department machines
# at Colorado State University. These systems have module support for a
# variety of different compilers. To compile on other systems which don't
# support the same modules, simply remove the "module" commands and
# update the compiler command to be your compiler of choice.

# Several program versions with the "-g" flag are commented out.
# This flag is only to be used with Intel Advisor to generate a Roofline plot
# which is able to point to the source code for each of the identified loops.

#####################################################################
# Setup Steps
#####################################################################

# All the build files are in the parent directory.
cd ..

# Remove any object files that might exist.
count=`ls -1 *.o 2>/dev/null | wc -l`
if [ $count != 0 ]; then
    rm *.o
fi 

# Remove any build artifacts that may exist.
mkdir -p bin
count=`ls -1 bin/* 2>/dev/null | wc -l`
if [ $count != 0 ]; then
    rm bin/*
fi

#####################################################################
# GCC version 6.5.0
#####################################################################

module purge
module load gcc/6.5.0

# Make the application with GCC version 6.5.0.
g++ -pthread -D_REENTRANT -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -lm *.o -o "bin/superego-6.5.0"
rm *.o
g++ -pthread -D_REENTRANT -DVALIDATE -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -DVALIDATE -lm *.o -o "bin/superego-6.5.0-validate"
rm *.o

# Make the application with GCC version 6.5.0 and AVX instructions.
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -lm *.o -o "bin/superego-6.5.0-avx"
rm *.o
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DVALIDATE -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DVALIDATE -lm *.o -o "bin/superego-6.5.0-avx-validate"
rm *.o


#####################################################################
# GCC version 13.2.0
#####################################################################

module purge
module load gcc/13.2.0

# Make the application with GCC version 13.2.0.
g++ -pthread -D_REENTRANT -DFIX -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -DFIX -lm *.o -o "bin/superego-13.2.0"
rm *.o
# g++ -g -pthread -D_REENTRANT -DFIX -Ofast -Wall -c *.cpp
# g++ -g -pthread -D_REENTRANT -DFIX -lm *.o -o "bin/superego-13.2.0-g"
# rm *.o
g++ -pthread -D_REENTRANT -DFIX -DVALIDATE -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -DFIX -DVALIDATE -lm *.o -o "bin/superego-13.2.0-validate"
rm *.o

# Make the application with GCC version 13.2.0 and AVX enabled.
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -lm *.o -o "bin/superego-13.2.0-avx"
rm *.o
# g++ -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -Ofast -Wall -c *.cpp
# g++ -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -lm *.o -o "bin/superego-13.2.0-avx-g"
# rm *.o
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DVALIDATE -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DVALIDATE -lm *.o -o "bin/superego-13.2.0-avx-validate"
rm *.o

# Make the improvement v1 application with GCC.
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE1 -lm *.o -o "bin/superego-gcc-improve1"
rm *.o
# g++ -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
# g++ -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -lm *.o -o "bin/superego-gcc-improve1-g"
# rm *.o
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -lm *.o -o "bin/superego-gcc-improve1-validate"
rm *.o

# Make the improvement v1 application with GCC and AVX enabled.
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -lm *.o -o "bin/superego-gcc-improve1-avx"
rm *.o
# g++ -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
# g++ -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -lm *.o -o "bin/superego-gcc-improve1-avx-g"
# rm *.o
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -lm *.o -o "bin/superego-gcc-improve1-avx-validate"
rm *.o

# Make the improvement v3 application with GCC.
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE3 -lm *.o -o "bin/superego-gcc-improve3"
rm *.o
# g++ -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
# g++ -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -lm *.o -o "bin/superego-gcc-improve3-g"
# rm *.o
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -Ofast -Wall -c *.cpp
g++ -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -lm *.o -o "bin/superego-gcc-improve3-validate"
rm *.o

# Make the improvement v3 application with GCC and AVX enabled.
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -lm *.o -o "bin/superego-gcc-improve3-avx"
rm *.o
# g++ -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
# g++ -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -lm *.o -o "bin/superego-gcc-improve3-avx-g"
# rm *.o
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -lm *.o -o "bin/superego-gcc-improve3-avx-validate"
rm *.o

# Make the improvement v4 application with GCC. AVX is required to be enabled.
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -lm *.o -o "bin/superego-gcc-improve4-avx"
rm *.o
# g++ -mavx -mavx2 -mfma -g -pthread -march=native -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
# g++ -mavx -mavx2 -mfma -g -pthread -march=native -D_REENTRANT -DFIX -DIMPROVE4 -lm *.o -o "bin/superego-gcc-improve4-avx-g"
# rm *.o
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE -Ofast -Wall -c *.cpp
g++ -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE -lm *.o -o "bin/superego-gcc-improve4-avx-validate"
rm *.o


#####################################################################
# ICC
#####################################################################

module purge
module load compilers/icc

# Make the application with ICC.
icc -pthread -D_REENTRANT -DFIX -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX *.o -o "bin/superego-icc"
rm *.o
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -Ofast -Wall -c *.cpp
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX *.o -o "bin/superego-icc-avx"
rm *.o
icc -pthread -D_REENTRANT -DFIX -DVALIDATE -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX -DVALIDATE *.o -o "bin/superego-icc-validate"
rm *.o

# Make the improvement v1 application with ICC.
icc -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icc-improve1"
rm *.o
# icc -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
# icc -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icc-improve1-g"
# rm *.o
icc -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE *.o -o "bin/superego-icc-improve1-validate"
rm *.o

# Make the improvement v1 application with ICC and AVX enabled.
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icc-improve1-avx"
rm *.o
# icc -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
# icc -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icc-improve1-avx-g"
# rm *.o
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -Ofast -Wall -c *.cpp
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE *.o -o "bin/superego-icc-improve1-avx-validate"
rm *.o

# Make the improvement v3 application with ICC.
icc -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icc-improve3"
rm *.o
# icc -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
# icc -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icc-improve3-g"
# rm *.o
icc -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE *.o -o "bin/superego-icc-improve3-validate"
rm *.o

# Make the improvement v3 application with ICC and AVX enabled.
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icc-improve3-avx"
rm *.o
# icc -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
# icc -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icc-improve3-avx-g"
# rm *.o
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -Ofast -Wall -c *.cpp
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE *.o -o "bin/superego-icc-improve3-avx-validate"
rm *.o

# Make the improvement v4 application with ICC.
icc -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icc-improve4"
rm *.o
# icc -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
# icc -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icc-improve4-g"
# rm *.o
icc -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE -Ofast -Wall -c *.cpp
icc -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE *.o -o "bin/superego-icc-improve4-validate"
rm *.o

# Make the improvement v4 application with ICC and AVX enabled.
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icc-improve4-avx"
rm *.o
# icc -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
# icc -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icc-improve4-g-avx"
# rm *.o
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE -Ofast -Wall -c *.cpp
icc -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE *.o -o "bin/superego-icc-improve4-avx-validate"
rm *.o


#####################################################################
# ICX
#####################################################################

module purge
module load compilers/icx

# Make the application with ICX.
icx -pthread -D_REENTRANT -DFIX -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX *.o -o "bin/superego-icx"
rm *.o
icx -pthread -D_REENTRANT -DFIX -DVALIDATE -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX -DVALIDATE *.o -o "bin/superego-icx-validate"
rm *.o

# Make the improvement v1 application with ICX.
icx -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icx-improve1"
rm *.o
# icx -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
# icx -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icx-improve1-g"
# rm *.o
icx -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE *.o -o "bin/superego-icx-improve1-validate"
rm *.o

# Make the improvement v1 application with ICX and AVX enabled.
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icx-improve1-avx"
rm *.o
# icx -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 -Ofast -Wall -c *.cpp
# icx -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE1 *.o -o "bin/superego-icx-improve1-avx-g"
# rm *.o
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE -Ofast -Wall -c *.cpp
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE1 -DVALIDATE *.o -o "bin/superego-icx-improve1-avx-validate"
rm *.o

# Make the improvement v3 application with ICX.
icx -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icx-improve3"
rm *.o
# icx -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
# icx -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icx-improve3-g"
# rm *.o
icx -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE *.o -o "bin/superego-icx-improve3-validate"
rm *.o

# Make the improvement v3 application with ICX and AVX enabled.
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icx-improve3-avx"
rm *.o
# icx -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 -Ofast -Wall -c *.cpp
# icx -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE3 *.o -o "bin/superego-icx-improve3-avx-g"
# rm *.o
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE -Ofast -Wall -c *.cpp
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE3 -DVALIDATE *.o -o "bin/superego-icx-improve3-avx-validate"
rm *.o

# Make the improvement v4 application with ICX.
icx -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icx-improve4"
rm *.o
# icx -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
# icx -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icx-improve4-g"
# rm *.o
icx -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE -Ofast -Wall -c *.cpp
icx -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE *.o -o "bin/superego-icx-improve4-validate"
rm *.o

# Make the improvement v4 application with ICX and AVX enabled.
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icx-improve4-avx"
rm *.o
# icx -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 -Ofast -Wall -c *.cpp
# icx -mavx -mavx2 -mfma -g -pthread -D_REENTRANT -DFIX -DIMPROVE4 *.o -o "bin/superego-icx-improve4-g-avx"
# rm *.o
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE -Ofast -Wall -c *.cpp
icx -mavx -mavx2 -mfma -pthread -D_REENTRANT -DFIX -DIMPROVE4 -DVALIDATE *.o -o "bin/superego-icx-improve4-avx-validate"
rm *.o
