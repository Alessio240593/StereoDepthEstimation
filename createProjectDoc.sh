#! /bin/bash

clear

if [ -d "project_doc" ]
then
    rm -rf project_doc
fi

mkdir project_doc
doxygen Doxyfile
