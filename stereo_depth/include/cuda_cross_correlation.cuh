#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda.h>
#include <thrust/device_vector.h>
#include <iostream>
#include <stdio.h>

#define DIM_BLOCK_X 3
#define DIM_BLOCK_Y 3


__host__ void checkCudaError(cudaError_t err, const char *func, int line)
{
    if(err != cudaSuccess) {
        std::cerr << std::endl << func << " error!" << "\nError → " << cudaGetErrorString(err) << "\nFile → " << __FILE__ << "\nLine → " << line << std::endl;
        exit(EXIT_FAILURE);
    }
}

template <typename T>
__global__ void crossCorrelationKernel(T              *d_src1, 
                                       T              *d_src2,
                                       T              *d_dest,
                                       std::size_t    kernel_size, 
                                       std::size_t    matrix_rows,
                                       std::size_t    matrix_cols)
{
    int row = blockDim.y * blockIdx.y + threadIdx.y;
    int col = blockDim.x * blockIdx.x + threadIdx.x;
    std::size_t max_idx{0};
    T max{0};
    T tmp;

    int pos = (row * matrix_cols + col) + (kernel_size / 2);

    if(pos < matrix_cols * matrix_rows - (kernel_size / 2)) {
        for (std::size_t i = kernel_size / 2; i < matrix_cols - kernel_size / 2; i++) {
            tmp = 0;
            for (std::size_t j = row - (kernel_size / 2); j <= row + (kernel_size / 2); j++) {
                for (std::size_t k = 0; k < kernel_size; k++) {
                    tmp += *(d_src1 + (j * matrix_cols) + i + k) * *(d_src2 + (j * matrix_cols) + k);
                }
            }

            if (tmp >= max) {
                max = tmp;
                max_idx = i - 1;
            } 
        }
        *(d_dest + pos - (kernel_size / 2)) = max_idx;
    }
}


template <typename T>
__host__ void crossCorrelation(T              *h_src1, 
                               T              *h_src2,
                               T              *h_dest,
                               std::size_t    kernel_size, 
                               std::size_t    matrix_rows,
                               std::size_t    matrix_cols)
{
    T *d_src1, *d_src2, *d_dest;

    std::size_t src_size{matrix_rows * matrix_cols * sizeof(T)};
    std::size_t dest_size{(matrix_rows - (kernel_size - 1)) * (matrix_cols - (kernel_size - 1)) * sizeof(T)};

    dim3 dimGrid(ceil(dest_size / DIM_BLOCK_X), ceil(dest_size / DIM_BLOCK_Y), 1);
    dim3 dimBlock(DIM_BLOCK_X, DIM_BLOCK_Y, 1);

    cudaError_t err = cudaMalloc((void**)&d_src1, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src1, h_src1, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    /*std::cout << "\n→ h_src1:\n";
    for (std::size_t i = 0; i < 5; i++) {
        for (std::size_t j = 0; j < 10; j++) {
            std::cout << +*(h_src1 +(i * 10) + j) << " ";
        }
        std::cout <<"\n";
    }

    thrust::device_ptr<T> dev_ptr_key = thrust::device_pointer_cast(d_src1);
    printf("\n→ d_src1:\n");
    for (std::size_t i = 0; i < 5; i++) {
        for (std::size_t j = 0; j < 10; j++) {
            std::cout << +((T)*(dev_ptr_key + (i * 10) + j)) << " ";
        }
        std::cout <<"\n";
    }*/

    err = cudaMalloc((void**)&d_src2, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src2, h_src2, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMalloc((void**)&d_dest, dest_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    crossCorrelationKernel<T><<<dimGrid, dimBlock>>>(d_src1, d_src2, d_dest, kernel_size, matrix_rows, matrix_cols);

    err = cudaMemcpy(h_dest, d_dest, dest_size, cudaMemcpyDeviceToHost);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    cudaFree(d_src1); cudaFree(d_src2); cudaFree(d_dest);
}