#! /bin/bash

clear

if [ ! -d "calibration_setup" ]
then
    mkdir calibration_setup
fi

if [ -d "build" ]
then
    rm -rf build
fi

mkdir build
cd build
cmake .. && make