/****************************************************************************
 * Copyright (C) 2022 by Alessio Zattoni                                    *
 *                                                                          *
 * This file is part of CrossCorrelation.                                   *
 *                                                                          *
 *   CrossCorrelation is free software: you can redistribute it and/or      *
 *   modify it under the terms of the GNU Lesser General Public License as  *
 *   published by the Free Software Foundation, either version 3 of the     * 
 *   License, or (at your option) any later version.                        * 
 *                                                                          *
 *   CrossCorrelation is distributed in the hope that it will be            *
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/



/**
 * @file cuda_cross_correlation.cuh
 * @author Alessio Zattoni
 * @date 
 * @brief Questo file contiene delle funzioni per eseguire la cross-correlazione in CUDA
 *
 * ...
 */



#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda.h>
#include <thrust/device_vector.h>
#include <iostream>
#include <stdio.h>



/**
 * @brief   Controlla se la chiamata a funzioni CUDA è andata a buon fine, altrimenti stampa un messaggio di errore
 * @note    err deve essere di tipo \p cudaError_t
 * 
 * @param[in]   err     Variabile per controllo errori
 * @param[in]   func    Nome della funzione
 * @param[out]  line    Linea della chiamata della funzione
 * 
 * 
 * 
 * @return void
*/
__host__ void checkCudaError(cudaError_t err, const char *func, int line)
{
    if (err != cudaSuccess) {
        std::cerr << std::endl << func << " error!" << "\nError → " << cudaGetErrorString(err) << "\nFile → " << __FILE__ << "\nLine → " << line << std::endl;
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief   Kernel per il calcolo della cross-correlazione, viene richiamato dalla funzione \p crossCorrelation
 * @note    err deve essere di tipo \p cudaError_t
 * 
 * @tparam      T                   Tipo delle matrici sorgenti e destinazione 
 * 
 * @param[in]   d_src1              Prima matrice di input
 * @param[in]   d_src2              Seconda matrice di input
 * @param[out]  d_dst               Matrice destinazione
 * @param[in]   kernel_size         Dimensione del kernel
 * @param[in]   matrix_src_rows     Altezza delle due matrici \p src1, \p src2
 * @param[in]   matrix_src_cols     Lunghezza delle due matrici \p src1, \p src2
 * 
 * 
 * 
 * @return void
*/
template <typename T>
__global__ void crossCorrelationKernel(T              *d_src1, 
                                       T              *d_src2,
                                       T              *d_dest,
                                       std::size_t    kernel_size, 
                                       std::size_t    matrix_src_rows,
                                       std::size_t    matrix_src_cols)
{
    int shift_on_axis = kernel_size / 2;
    int sRow = blockDim.y * blockIdx.y + threadIdx.y + shift_on_axis;
    int sCol = blockDim.x * blockIdx.x + threadIdx.x + shift_on_axis;
    int dRow = blockDim.y * blockIdx.y + threadIdx.y;
    int dCol = blockDim.x * blockIdx.x + threadIdx.x;

    int matrix_dest_cols = matrix_src_cols - (kernel_size - 1);
    int matrix_dest_rows = matrix_src_rows - (kernel_size - 1);
    int dPos = dRow * matrix_dest_cols + dCol;
    std::size_t max_idx{0};
    T max{0};
    T tmp;

    if (dRow < matrix_dest_rows && dCol < matrix_dest_cols) {
        for (std::size_t i = shift_on_axis; i < matrix_src_cols - shift_on_axis; i++) {
            tmp = 0;
            for (std::size_t j = sRow - shift_on_axis; j <= sRow + shift_on_axis; j++) {
                for (std::size_t k = 0, l = sCol - shift_on_axis; k < kernel_size, l <= sCol + shift_on_axis; k++, l++) {
                    tmp += *(d_src1 + (j * matrix_src_cols) + k + (i - shift_on_axis)) * *(d_src2 + (j * matrix_src_cols) + l);
                }
            }

            if (tmp >= max) {
                max = tmp;
                max_idx = i - 1;
            } 
        }
        *(d_dest + dPos) = max_idx;
    }
}



template <typename T>
__global__ void sharedMemoryCrossCorrelationKernel(T              *d_src1, 
                                                   T              *d_src2,
                                                   T              *d_dest,
                                                   std::size_t    kernel_size, 
                                                   std::size_t    matrix_src_rows,
                                                   std::size_t    matrix_src_cols)
        {

    int shift_on_axis = kernel_size / 2;
    int sRow = blockDim.y * blockIdx.y + threadIdx.y;
    int sCol = blockDim.x * blockIdx.x + threadIdx.x;
    int dRow = blockDim.y * blockIdx.y + threadIdx.y - shift_on_axis;
    int dCol = blockDim.x * blockIdx.x + threadIdx.x - shift_on_axis;

    int pos = sRow * matrix_src_cols + sCol;

    extern __shared__ int d_shmem1[];
    extern __shared__ int d_shmem2[];

    d_shmem1[pos] = d_src1[pos];
    d_shmem2[pos] = d_src2[pos];

    __syncthreads();

    int matrix_dest_cols = matrix_src_cols - (kernel_size - 1);
    int matrix_dest_rows = matrix_src_rows - (kernel_size - 1);
    int dPos = dRow * matrix_dest_cols + dCol;
    std::size_t max_idx{0};
    T max{0};
    T tmp;

    if (dRow < matrix_dest_rows && dCol < matrix_dest_cols) {
        for (std::size_t i = shift_on_axis; i < matrix_src_cols - shift_on_axis; i++) {
            tmp = 0;
            for (std::size_t j = sRow - shift_on_axis; j <= sRow + shift_on_axis; j++) {
                for (std::size_t k = 0, l = sCol - shift_on_axis; k < kernel_size, l <= sCol + shift_on_axis; k++, l++) {
                    tmp += *(d_shmem1 + (j * matrix_src_cols) + k + (i - shift_on_axis)) * *(d_shmem2 + (j * matrix_src_cols) + l);
                }
            }

            if (tmp >= max) {
                max = tmp;
                max_idx = i - 1;
            } 
        }
        *(d_dest + dPos) = max_idx;
    }
    
}


template <typename T>
__host__ void sharedMemoryCrossCorrelation(T              *h_src1, 
                                           T              *h_src2,
                                           T              *h_dest,
                                           std::size_t    kernel_size, 
                                           std::size_t    matrix_rows,
                                           std::size_t    matrix_cols,
                                           std::size_t    block_dim_x,
                                           std::size_t    block_dim_y)
{
    T *d_src1, *d_src2, *d_dest;

    std::size_t src_size{matrix_rows * matrix_cols * sizeof(T)};
    float dest_rows = matrix_rows - (kernel_size - 1);
    float dest_cols = matrix_cols - (kernel_size - 1);
    std::size_t dest_size{dest_rows * dest_cols * sizeof(T)};
    
    dim3 dimGrid(ceil(float(matrix_cols) / block_dim_y), ceil(float(matrix_rows) / block_dim_x), 1);
    dim3 dimBlock(block_dim_y, block_dim_x, 1);

    cudaError_t err = cudaMalloc((void**)&d_src1, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src1, h_src1, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMalloc((void**)&d_src2, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src2, h_src2, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMalloc((void**)&d_dest, dest_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    sharedMemoryCrossCorrelationKernel<T><<<dimGrid, dimBlock, (matrix_cols * matrix_rows)>>>(d_src1, d_src2, d_dest, kernel_size, matrix_rows, matrix_cols);

    cudaDeviceSynchronize();

    err = cudaMemcpy(h_dest, d_dest, dest_size, cudaMemcpyDeviceToHost);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    cudaFree(d_src1); cudaFree(d_src2); cudaFree(d_dest);
}



/**
 * @brief Calcola la cross-correlazione tra \p src1 e \p src2 con un kernel di dimensione \p kernel_size X \p kernel_size.
 * @note  → Le matrici \p src1 e \p src2 devono avere dimensione \p height X \p width. \n
 *        → Le matrici \p src1 e \p src2 possono avere altezza maggiore o uguale a \p kernel_size. \n
 *        → Il kernel deve avere una dimensione dispari e deve essere una matrice quadrata. \n
 *        → La matrice destinazione deve avere dimensione (src_width - (kernel_size - 1)) * (src_height - (kernel_size - 1)). \n
 * 
 * @tparam      T               Tipo delle matrici sorgenti e destinazione 
 * 
 * @param[in]   h_src1          Prima matrice di input
 * @param[in]   h_src2          Seconda matrice di input
 * @param[out]  h_dst           Matrice destinazione
 * @param[in]   kernel_size     Dimensione del kernel
 * @param[in]   matrix_rows     Altezza delle due matrici \p src1, \p src2
 * @param[in]   matrix_cols     Lunghezza delle due matrici \p src1, \p src2
 * @param[in]   block_dim_x     Dimensioni del blocco di thread lungo la dimensione x
 * @param[in]   block_dim_y     Dimensioni del blocco di thread lungo la dimensione y
 * 
 * 
 * 
 * @return void
*/
template <typename T>
__host__ void crossCorrelation(T              *h_src1, 
                               T              *h_src2,
                               T              *h_dest,
                               std::size_t    kernel_size, 
                               std::size_t    matrix_rows,
                               std::size_t    matrix_cols, 
                               std::size_t    block_dim_x,
                               std::size_t    block_dim_y)
{
    T *d_src1, *d_src2, *d_dest;

    std::size_t src_size{matrix_rows * matrix_cols * sizeof(T)};
    float dest_rows = matrix_rows - (kernel_size - 1);
    float dest_cols = matrix_cols - (kernel_size - 1);
    std::size_t dest_size{dest_rows * dest_cols * sizeof(T)};
    
    dim3 dimGrid(ceil(dest_cols / block_dim_y), ceil(dest_rows / block_dim_x), 1);
    dim3 dimBlock(block_dim_y, block_dim_x, 1);

    cudaError_t err = cudaMalloc((void**)&d_src1, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src1, h_src1, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMalloc((void**)&d_src2, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src2, h_src2, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMalloc((void**)&d_dest, dest_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    crossCorrelationKernel<T><<<dimGrid, dimBlock>>>(d_src1, d_src2, d_dest, kernel_size, matrix_rows, matrix_cols);

    cudaDeviceSynchronize();

    err = cudaMemcpy(h_dest, d_dest, dest_size, cudaMemcpyDeviceToHost);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    cudaFree(d_src1); cudaFree(d_src2); cudaFree(d_dest);
}


/**
 * @brief Calcola la cross-correlazione tra \p src1 e \p src2 con un kernel di dimensione \p kernel_size X \p kernel_size.
 * @note  → Le matrici \p src1 e \p src2 devono avere dimensione \p height X \p width. \n
 *        → Le matrici \p src1 e \p src2 possono avere altezza maggiore o uguale a \p kernel_size. \n
 *        → Il kernel deve avere una dimensione dispari e deve essere una matrice quadrata. \n
 *        → La matrice destinazione deve avere dimensione (src_width - (kernel_size - 1)) * (src_height - (kernel_size - 1)). \n
 * 
 * @tparam      T               Tipo delle matrici sorgenti e destinazione 
 * 
 * @param[in]   h_src1          Prima matrice di input
 * @param[in]   h_src2          Seconda matrice di input
 * @param[out]  h_dst           Matrice destinazione
 * @param[in]   kernel_size     Dimensione del kernel
 * @param[in]   matrix_rows     Altezza delle due matrici \p src1, \p src2
 * @param[in]   matrix_cols     Lunghezza delle due matrici \p src1, \p src2
 * @param[in]   block_dim_x     Dimensioni del blocco di thread lungo la dimensione x
 * @param[in]   block_dim_y     Dimensioni del blocco di thread lungo la dimensione y
 * 
 * 
 * @return void
*/
template <typename T>
__host__ double crossCorrelationTimeWithAllocation(T              *h_src1, 
                                                   T              *h_src2,
                                                   T              *h_dest,
                                                   std::size_t    kernel_size, 
                                                   std::size_t    matrix_rows,
                                                   std::size_t    matrix_cols,
                                                   std::size_t    dim_block_x,
                                                   std::size_t    dim_block_y)
{
    T *d_src1, *d_src2, *d_dest;

    std::size_t src_size{matrix_rows * matrix_cols * sizeof(T)};
    float dest_rows = matrix_rows - (kernel_size - 1);
    float dest_cols = matrix_cols - (kernel_size - 1);
    std::size_t dest_size{dest_rows * dest_cols * sizeof(T)};
    
    dim3 dimGrid(ceil(dest_cols / dim_block_y), ceil(dest_rows / dim_block_x), 1);
    dim3 dimBlock(dim_block_y, dim_block_x, 1);

    cudaError_t err = cudaMalloc((void**)&d_src1, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMalloc((void**)&d_src2, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMalloc((void**)&d_dest, dest_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    // Prepare
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // TODO spostare malloc sotto e includere solo le memcpy nel calcolo del tempo

    // Start record
    cudaEventRecord(start, 0);

    err = cudaMemcpy(d_src1, h_src1, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMemcpy(d_src2, h_src2, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    crossCorrelationKernel<T><<<dimGrid, dimBlock>>>(d_src1, d_src2, d_dest, kernel_size, matrix_rows, matrix_cols);

    err = cudaMemcpy(h_dest, d_dest, dest_size, cudaMemcpyDeviceToHost);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    // Stop event
    cudaEventRecord(stop, 0);

    cudaEventSynchronize(stop);

    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop); // that's our time!

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_src1); cudaFree(d_src2); cudaFree(d_dest);

    return elapsedTime;
}


/**
 * @brief Calcola la cross-correlazione tra \p src1 e \p src2 con un kernel di dimensione \p kernel_size X \p kernel_size.
 * @note  → Le matrici \p src1 e \p src2 devono avere dimensione \p height X \p width. \n
 *        → Le matrici \p src1 e \p src2 possono avere altezza maggiore o uguale a \p kernel_size. \n
 *        → Il kernel deve avere una dimensione dispari e deve essere una matrice quadrata. \n
 *        → La matrice destinazione deve avere dimensione (src_width - (kernel_size - 1)) * (src_height - (kernel_size - 1)). \n
 * 
 * @tparam      T               Tipo delle matrici sorgenti e destinazione 
 * 
 * @param[in]   h_src1          Prima matrice di input
 * @param[in]   h_src2          Seconda matrice di input
 * @param[out]  h_dst           Matrice destinazione
 * @param[in]   kernel_size     Dimensione del kernel
 * @param[in]   matrix_rows     Altezza delle due matrici \p src1, \p src2
 * @param[in]   matrix_cols     Lunghezza delle due matrici \p src1, \p src2
 * @param[in]   block_dim_x     Dimensioni del blocco di thread lungo la dimensione x
 * @param[in]   block_dim_y     Dimensioni del blocco di thread lungo la dimensione y
 * 
 * 
 * @return void
*/
template <typename T>
__host__ double crossCorrelationTimeWithoutAllocation(T             *h_src1, 
                                                      T              *h_src2,
                                                      T              *h_dest,
                                                      std::size_t    kernel_size, 
                                                      std::size_t    matrix_rows,
                                                      std::size_t    matrix_cols,
                                                      std::size_t    dim_block_x,
                                                      std::size_t    dim_block_y)
{
    T *d_src1, *d_src2, *d_dest;

    std::size_t src_size{matrix_rows * matrix_cols * sizeof(T)};
    float dest_rows = matrix_rows - (kernel_size - 1);
    float dest_cols = matrix_cols - (kernel_size - 1);
    std::size_t dest_size{dest_rows * dest_cols * sizeof(T)};
    
    dim3 dimGrid(ceil(dest_cols / dim_block_y), ceil(dest_rows / dim_block_x), 1);
    dim3 dimBlock(dim_block_y, dim_block_x, 1);

    cudaError_t err = cudaMalloc((void**)&d_src1, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src1, h_src1, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMalloc((void**)&d_src2, src_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    err = cudaMemcpy(d_src2, h_src2, src_size, cudaMemcpyHostToDevice);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    err = cudaMalloc((void**)&d_dest, dest_size);
    checkCudaError(err, "cudaMalloc", __LINE__);

    // Prepare
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Start record
    cudaEventRecord(start, 0);

    crossCorrelationKernel<T><<<dimGrid, dimBlock>>>(d_src1, d_src2, d_dest, kernel_size, matrix_rows, matrix_cols);

    // Stop event
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop); // that's our time!

    err = cudaMemcpy(h_dest, d_dest, dest_size, cudaMemcpyDeviceToHost);
    checkCudaError(err, "cudaMemcpy", __LINE__);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_src1); cudaFree(d_src2); cudaFree(d_dest);

    return elapsedTime;
}