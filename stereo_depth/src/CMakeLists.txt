set(LIBRARY_NAME stereodepth-core)

add_library(${LIBRARY_NAME} OBJECT
    math.cpp
    stereo_image.cpp
    cross_correlation.cpp
    cuda_cross_correlation.cu
)
