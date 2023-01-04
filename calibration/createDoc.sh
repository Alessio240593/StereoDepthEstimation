#! /bin/bash

clear

if [ -d "doc" ]
then
    rm -rf doc
fi

mkdir doc
doxygen Doxyfile
