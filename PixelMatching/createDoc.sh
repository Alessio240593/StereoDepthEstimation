#! /bin/bash

if [ -d "doc" ]
then
    rm -rf doc
fi

mkdir doc
clear
doxygen Doxyfile