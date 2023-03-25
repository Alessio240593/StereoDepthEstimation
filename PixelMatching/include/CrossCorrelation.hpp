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
 * @file CrossCorrelation.hpp
 * @author Alessio Zattoni
 * @date 
 * @brief Questo file contiene delle funzioni per eseguire la cross-correlazione
 *
 * ...
 */



#pragma once

#include <cstddef>
#include <iostream>
#include <cstdlib>

/// Minima dimensione del kernel
#define KERNEL_LIMIT 3


/**
 * @brief Esegue il padding della matrice \p src.
 * @note  → La dimensione della matrice \p src deve essere maggiore della dimensione del kernel. \n
 *        → Il kernel deve avere dimensione minima definita come \p KERNEL_LIMIT X \p KERNEL_LIMIT. \n
 *        → La dimensione della matrice \p src deve essere almeno \p KERNEL_LIMIT X \p KERNEL_LIMIT. \n
 *        → La matrice \p dst deve avere dimensione \p src_rows + (padding * 2) dove padding è definito come \p kernel_size / 2. \n
 *        → I parametri \p kernel_rows e \p kernel_cols devono avere la stessa dimensione, inoltre devono essere dispari. \n
 * 
 * @tparam      T             Tipo della matrice sorgente, della matrice destinazione
 * 
 * @param[in]   src           Matrice di input
 * @param[out]  dst           Matrice di destinazione
 * @param[in]   src_rows      Numero di righe della matrice sorgente
 * @param[in]   src_cols      Numero di colonne della matrice sorgente
 * @param[in]   kernel_rows   Numero di righe del kernel
 * @param[in]   kernel_cols   Numero di colonne del kernel
 * 
 * @return      void
*/
template <typename T>
void padding(const T            *src, 
             T                  *dst, 
             const std::size_t  src_rows, 
             const std::size_t  src_cols, 
             const std::size_t  kernel_rows, 
             const std::size_t  kernel_cols)
{
    const std::size_t input_size = src_rows * src_cols;
    const std::size_t kernel_size = kernel_rows * kernel_cols;
    
    if (src_rows < KERNEL_LIMIT || src_cols < KERNEL_LIMIT) {
        std::cerr << "Input size must be greater than " << KERNEL_LIMIT << " X " << KERNEL_LIMIT <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE);
    }

    if (kernel_size < KERNEL_LIMIT * KERNEL_LIMIT) {
        std::cerr << "Kernel size must be greater than " << KERNEL_LIMIT << " X " << KERNEL_LIMIT <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE);  
    } 

    if (input_size < kernel_size) {
        std::cerr << "Input size must be greater than kernel size " <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE);      
    }

    if (!src || !dst) {
        std::cerr << "Invalid source or destination matrix" <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE); 
    }

    const std::size_t padding_cols = kernel_cols / 2;
    const std::size_t padding_rows = kernel_rows / 2;
    const std::size_t dst_rows = src_rows + (padding_rows * 2);
    const std::size_t dst_cols = src_cols + (padding_cols * 2);

    for (std::size_t i = 0; i < dst_rows; i++) {
        for (std::size_t j = 0; j < dst_cols; j++) {
            //*( dst + (i * (src_cols + padding_size)) + j)  = *(src + ((i - row_pos) * (src_cols)) +  (j - col_pos));
            //std::cout << int(*( tmp + i * cols + j)) << " ";
            if (i < padding_rows || i > src_rows ||
                j < padding_cols || j > src_cols) {
                *(dst + (i * dst_cols) + j) = T{0};
            }
            else {
                *(dst + (i * dst_cols) + j) = *(src + (src_cols * (i - padding_cols)) + (j - padding_rows));
            }
        }
        //std::cout << std::endl;
    }
    //std::cout << std::endl;
}


/**
 * @brief Controlla se i parametri in input sono accettati dalla funzione
 *
 * @tparam      T             Tipo della matrice sorgente e della matrice kernel
 * 
 * @param[in]   src           Matrice di input
 * @param[out]  kernel        Matrice kernel
 * @param[in]   kernel_size   Dimensione della matrice kernel
 * @param[in]   matrix_width  Lunghezza della matrice sorgente
 * 
 * @return      void
*/
template <typename T>
void inputParsing(const T           *src, 
                  const T           *kernel, 
                  const std::size_t kernel_size, 
                  const std::size_t matrix_width)
{
    if (kernel_size < 3) {
        std::cerr << "Matrix width must be greater than " << KERNEL_LIMIT <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;
        exit(EXIT_FAILURE);
    }

    if (matrix_width < 3) {
        std::cerr << "Kernel size must be greater than " << KERNEL_LIMIT <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!src) {
        std::cerr << "Invalid source matrix:" <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!kernel) {
        std::cerr << "Invalid kernel matrix: " <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief Calcola la cross-correlazione tra la matrice sorgente \p src e il kernel prelevato dalla seconda matrice sorgente \p kernel.
 * @note  → La matrice \p src e il \p kernel devono avere la stessa altezza. \n
 *        → La matrice \p src deve avere lunghezza maggiore o uguale a \p KERNEL_LIMIT. \n
 *        → Il kernel deve avere dimensione minima definita come \p KERNEL_LIMIT X KERNEL_LIMIT. \n
 *        → il kernel deve avere una dimensione dispari e deve essere una matrice quadrata. \n
 *
 * @tparam      T               Tipo della matrice sorgente e della matrice kernel
 * 
 * @param[in]   src             Matrice di input
 * @param[out]  kernel          Matrice kernel
 * @param[in]   kernel_size     Dimensione della matrice kernel
 * @param[in]   matrix_width    Lunghezza della matrice sorgente
 * 
 * @return Ritorna la posizione in cui la cross-correlazione assume il massimo valore.
 * @retval std::size_t
*/
template <typename T>
std::size_t argMaxCorr(const T              *src, 
                       const T              *kernel, 
                       const std::size_t    kernel_size, 
                       const std::size_t    matrix_width)
{
    inputParsing(src, kernel, kernel_size, matrix_width);

    T max{0};
    T tmp;
    std::size_t max_idx{0};
    const std::size_t pos = kernel_size / 2;

    for (std::size_t i = pos; i < matrix_width - pos; i++) {
        tmp = 0;
        for (std::size_t j = 0; j < kernel_size; j++) {
            for (std::size_t k = 0; k < kernel_size; k++) {
                tmp += *(src + (j * matrix_width) + i - pos + k) * *(kernel + (j * kernel_size) + k);
                //std::cout << "tmp→ " << tmp << "\n";
                //std::cout << "(" << *(src + (j * width) + i - 1 + k) << "," << *(kernel + (j * kernel_size) + k) << ")---";
            }
        }
        //std::cout << "\n\n";
        if (tmp >= max) {
            max = tmp;
            //std::cout << "max: " << max << " max_idx: " << max_idx << "\n";
            max_idx = i - 1;
        } 
    }

    return max_idx;
}

template <typename T>
std::size_t argMaxCorrV2(const T             *src1, 
                        const T              *src2, 
                        const std::size_t    offset,
                        const std::size_t    kernel_size, 
                        const std::size_t    matrix_width)
{
    //inputParsing(src, kernel, kernel_size, matrix_width);

    T max{0};
    T tmp;
    std::size_t max_idx{0};
    const std::size_t pos = kernel_size / 2;

    for (std::size_t i = pos; i < matrix_width - pos; i++) {
        tmp = 0;
        for (std::size_t j = 0; j < kernel_size; j++) {
            for (std::size_t k = 0; k < kernel_size; k++) {
                tmp += *(src1 + (j * matrix_width) + i - pos + k) * *(src2 + (j * matrix_width) + k + offset);
                //std::cout << "tmp→ " << tmp << "\n";
                //std::cout << "(" << *(src + (j * width) + i - 1 + k) << "," << *(kernel + (j * kernel_size) + k) << ")---";
            }
        }
        //std::cout << "\n\n";
        if (tmp >= max) {
            max = tmp;
            //std::cout << "max: " << max << " max_idx: " << max_idx << "\n";
            max_idx = i - 1;
        } 
    }

    return max_idx;
}


/**
 * @brief Copia nella matrice \p kernel una porzione della matrice \p src della stessa grandezza di \p kernel.
 * @note  → La matrice \p src e la matrice \p kernel devono avere la stessa altezza. \n
 *        → Il kernel deve avere una lunghezza minore o uguale della matrice \p src, inoltre deve essere almeno \p KERNEL_LIMIT X \p KERNEL_LIMIT. \n
 *        → il kernel deve avere una dimensione dispari e deve essere una matrice quadrata. \n
 * 
 * @tparam      T               Tipo della matrice sorgente e della matrice kernel
 * 
 * @param[in]   src             Prima matrice di input
 * @param[out]  kernel          Matrice kernel
 * @param[in]   pos             Indica l'offset del kernel all'interno della matrice \p src 
 * @param[in]   kernel_size     Dimensione della matrice kernel
 * @param[in]   matrix_width    Lunghezza della matrice sorgente
 * 
 * @return void
*/
template <typename T>
void copySrcToKernel(const T            *src, 
                     T                  *kernel, 
                     const std::size_t  pos, 
                     const std::size_t  kernel_size, 
                     const std::size_t  matrix_width)
{
    inputParsing(src, kernel, kernel_size, matrix_width);

    for (std::size_t i = 0; i < kernel_size; i++) {
        for (std::size_t j = 0; j < kernel_size; j++) {
            *(kernel + (i * kernel_size) + j) = *(src + (i * matrix_width) + j + pos);
        }
    }

    /*for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            std::cout << +*(kernel + (i * kernel_size) + j ) << " ";
        }
        std::cout <<"\n";
    }
    std::cout <<"\n\n";*/
}


/**
 * @brief Copia nella matrice \p src_kernel_rows il contenuto di \p src a partire dalla riga \p current_row fino 
 * alla riga \p kernel_size + \p current_row - 1.
 * @note  → La matrice \p src e la matrice \p src_kernel_rows devono avere la stessa larghezza ( \p matrix_width). \n
 *        → \p kernel_size deve essere maggiore o uguale della dimensione \p KERNEL_LIMIT X \p KERNEL_LIMIT. \n
 *        → \p matrix width deve essere maggiore o uguale a \p kernel_size. \n
 * 
 * @tparam      T               Tipo della matrice sorgente e della matrice destinazione
 * 
 * @param[in]   src             Prima matrice di input
 * @param[out]  src_kernel_rows Matrice destinazione 
 * @param[in]   current_row     Indica la riga di partenza del processo di copia
 * @param[in]   kernel_size     Dimensione della matrice kernel
 * @param[in]   matrix_width    Lunghezza della matrice sorgente
 * 
 * @return void
*/
template <typename T>
void copySrcToSrcKernelRows(const T             *src, 
                            T                   *src_kernel_rows,
                            const std::size_t   current_row, 
                            const std::size_t   kernel_size, 
                            const std::size_t   matrix_width)
{
    for (std::size_t i = 0; i < kernel_size; i++) {
        for (std::size_t j = 0; j < matrix_width; j++) {
            *(src_kernel_rows + (i * matrix_width) + j) = *(src + ((i + current_row) * matrix_width) + j);
        }
    }

    /*for (size_t i = current_row; i < kernel_size + current_row; i++) {
        for (size_t j = 0; j < matrix_width; j++) {
            std::cout << +*(src_kernel_rows + (i * matrix_width) + j ) << " ";
        }
        std::cout <<"\n";
    }
    std::cout <<"\n\n";*/
}


/**
 * @brief Concatena il contenuto del vettore \p src_vect alla riga \p current_row nella matrice di destinazione \p dst 
 * @note  → La matrice \p dst deve avere la stessa lunghezza di \p src_vect. \n
 *        → La matrice \p dst deve essere di dimensione (src_width - (kernel_size - 1)) * (src_height - (kernel_size - 1)). \n
 * 
 * @tparam      T           Tipo della matrice destinazione e del vettore sorgente
 * 
 * @param[in]   src_vect    Vettore sorgente 
 * @param[out]  dst         Matrice di destinazione
 * @param[in]   dst_width   Lunghezza della matrice destinazione e del vettore sorgente
 * @param[in]   current_row Indica la riga dove copiare il vettore \p src_vect
 * 
 * @return void
*/
template <typename T>
void concatDst(const T              *src_vect,
               T                    *dst, 
               const std::size_t    dst_width, 
               const std::size_t    current_row)
{
    for (std::size_t i = 0; i < dst_width; i++) {
        *(dst + (current_row * dst_width) + i) = *(src_vect + i);
    }
}


/**
 * @brief Calcola la cross-correlazione tra \p src1 e \p src2 con un kernel di dimensione \p height X \p height.
 * @note  → Le matrici \p src1 e \p src2 devono avere dimensione \p height X \p width. \n
 *        → Le due matrici \p src1 e \p src2 devono avere la stessa atezza del kernel. \n
 *        → Il vettore destinazione \p dst deve avere dimensione \p width - ( \p height - 1). \n
 * 
 * @tparam      T         Tipo delle matrici sorgenti e destinazione 
 * 
 * @param[in]   src1      Prima matrice di input
 * @param[in]   src2      Seconda matrice di input
 * @param[out]  dst       Vettore destinazione
 * @param[in]   height    Dimensione del kernel, altezza delle due matrici \p src1, \p src2
 * @param[in]   width     Lunghezza delle due matrici
 * 
 * @return void
*/
template <typename T>
void argMaxCorrVector(const T           *src1, 
                      const T           *src2, 
                      T                 *dst, 
                      const std::size_t height, 
                      const std::size_t width)
{
    inputParsing(src1, src2, height, width);

    if (!dst) {
        std::cerr << "Invalid destination matrix" <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE); 
    }

    /*T *k = new(std::nothrow) T[height * height];

    if (!k) {
        std::cerr << "Memory allocation failed" <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE); 
    }*/

    for (std::size_t i = 0; i < width - (height - 1); i++) {
        //V1
        //copySrcToKernel<T>(src2, k, i, height, width);
        //*(dst + i) = argMaxCorr<T>(src1, k, height, width);
        //V2 senza copia
        *(dst + i) = argMaxCorrV2<T>(src1, src2, i, height, width);
    }

    //delete[] k;
}

/**
 * @brief Calcola la cross-correlazione tra \p src1 e \p src2 con un kernel di dimensione \p height X \p height.
 * @note  → Le matrici \p src1 e \p src2 devono avere dimensione \p height X \p width. \n
 *        → Le matrici \p src1 e \p src2 possono avere altezza maggiore o uguale a \p kernel_size. \n
 *        → Il kernel deve avere una dimensione dispari e deve essere una matrice quadrata. \n
 *        → La matrice destinazione deve avere dimensione (src_width - (kernel_size - 1)) * (src_height - (kernel_size - 1)). \n
 * 
 * @tparam      T           Tipo delle matrici sorgenti e destinazione 
 * 
 * @param[in]   src1        Prima matrice di input
 * @param[in]   src2        Seconda matrice di input
 * @param[out]  dst         Matrice destinazione
 * @param[in]   width       Lunghezza delle due matrici \p src1, \p src2
 * @param[in]   height      Altezza delle due matrici \p src1, \p src2
 * @param[in]   kernel_size Dimensione del kernel
 * 
 * @return void
*/
template <typename T>
void argMaxCorrMat(const T              *src1, 
                   const T              *src2, 
                   T                    *dst, 
                   const std::size_t    width, 
                   const std::size_t    height, 
                   const std::size_t    kernel_size)
{
    const std::size_t dst_vect_size = width - (kernel_size - 1);

    for (std::size_t i = 0; i < (height - kernel_size) + 1; i++) {
        argMaxCorrVector<T>(
            src1 + (i * width), 
            src2 + (i * width), 
            dst + (i * dst_vect_size), 
            kernel_size, width);
    }
}