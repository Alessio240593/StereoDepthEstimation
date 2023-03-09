set(LIBRARY_NAME stereodepth-core)

add_library(${LIBRARY_NAME} OBJECT
    cross_correlation.cpp
    cross_correlation.cu
)
