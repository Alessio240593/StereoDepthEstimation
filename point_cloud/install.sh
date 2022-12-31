#! /bin/bash

clear

if [ -d "build" ]
then
    rm -rf build
fi

if [ -d "pcd_images" ]
then
    rm -rf pcd_images
fi

mkdir build
mkdir pcd_images
cd build
cmake .. && make
