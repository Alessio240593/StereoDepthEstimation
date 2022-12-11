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
 * @brief Esegue il padding della matrice src
 * @note  La dimensione della matrice di input deve essere maggiore della dimensione del kernel, il kernel deve avere dimensione minima
 *        definita come KERNEL_LIMIT X KERNEL_LIMIT, la dimensione della matrice sorgente deve essere almeno KERNEL_LIMIT X KERNEL_LIMIT
 * 
 * @tparam      T             Tipo della matrice sorgente, della matrice destinazione e della matrice kernel
 * 
 * @param[in]   src           Matrice di input
 * @param[out]  dst           Matrice di destinazione con paddding
 * @param[in]   src_rows      Numero di righe della matrice sorgente
 * @param[in]   src_cols      Numero di colonne della matrice sorgente
 * @param[in]   dst_rows      Numero di righe della matrice destinazione
 * @param[in]   dst_cols      Numero di colonne della matrice destinazione
 * @param[in]   kernel_rows   Numero di righe del kernel
 * @param[in]   kernel_cols   Numero di colonne del kernel
 * 
 * @return      void
*/
//sistemare il padding
template <typename T>
void padding(const T           *src, 
             T                 *dst, 
             const std::size_t src_rows, 
             const std::size_t src_cols, 
             const std::size_t dst_rows, 
             const std::size_t dst_cols, 
             const std::size_t kernel_rows, 
             const std::size_t kernel_cols)
{
    const size_t input_size = src_rows * src_cols;
    const size_t kernel_size = kernel_rows * kernel_cols;
    
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

    if (dst_cols < src_cols + (kernel_cols - 1) || dst_rows < src_rows + (kernel_rows - 1)) {
        std::cerr << "Invalid dst size, size must be src size + padding" <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE); 
    }

    const std::size_t padding_size = kernel_cols - 1;
    const size_t row_pos = kernel_rows / 2;
    const size_t col_pos = kernel_cols / 2;

    for (size_t i = row_pos; i < src_rows + row_pos; i++) {
        for (size_t j = col_pos; j < src_cols + col_pos; j++) {
            *( dst + (i * (src_cols + padding_size)) + j)  = *(src + ((i - row_pos) * (src_cols)) +  (j - col_pos));
            //std::cout << int(*( tmp + i * cols + j)) << " ";
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
 * @brief Calcola la cross-correlazione tra la matrice sorgente e il kernel prelevato dalla seconda matrice sorgente
 * @note  La matrice src e il kernel devono avere la stessa altezza, il kernel deve essere più piccolo della matrice
 *        sorgente, il kernel deve avere una dimensione dispari e deve essere una matrice quadrata
 *
 * @tparam      T               Tipo della matrice sorgente e della matrice kernel
 * 
 * @param[in]   src             Matrice di input
 * @param[out]  kernel          Matrice kernel
 * @param[in]   kernel_size     Dimensione della matrice kernel
 * @param[in]   matrix_width    Lunghezza della matrice sorgente
 * 
 * @return Ritorna la posizione in cui la cross-correlazione assume il massimo valore
 * @retval std::size_t
*/
template <typename T>
std::size_t argMaxCorr(const T           *src, 
                       T                 *kernel, 
                       const std::size_t kernel_size, 
                       const std::size_t matrix_width)
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
                tmp += *(src + (j * matrix_width) + i - 1 + k) * *(kernel + (j * kernel_size) + k);
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
 * @brief Copia nella matrice kernel una porzione di src della stessa grandezza.
 * @note  Matrice src e il kernel devono avere la stessa altezza, il kernel deve essere più piccolo della matrice
 *        sorgente, il kernel deve avere una dimensione dispari e deve essere una matrice quadrata
 * 
 * @tparam      T               Tipo della matrice sorgente e della matrice kernel
 * 
 * @param[in]   src             Prima matrice di input
 * @param[out]  kernel          Matrice kernel
 * @param[in]   pos             Indica la posizione del pixel centrale  
 * @param[in]   kernel_size     Dimensione della matrice kernel
 * @param[in]   matrix_width    Lunghezza della matrice sorgente
 * 
 * @return void
*/
template <typename T>
void copyElements(const T           *src, 
                  T                 *kernel, 
                  std::size_t       pos, 
                  const std::size_t kernel_size, 
                  const std::size_t matrix_width)
{
    inputParsing(src, kernel, kernel_size, matrix_width);

    for (size_t i = 0; i < kernel_size; i++) {
        for (size_t j = 0; j < kernel_size; j++) {
            *(kernel + (i * kernel_size) + j) = *(src + (i * matrix_width) + j + pos);
        }
    }

    /*for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            std::cout << *(k + (i * kernel_width) + j ) << " ";
        }
        std::cout <<"\n";
    }
    std::cout <<"\n\n";*/
}


/**
 * @brief Calcola la cross-correlazione tra src1 e src2 con un kernel di dimensione height X height
 * @note  Le matrici src1 e src2 devono avere dimensione height X width
 * 
 * @tparam      T         Tipo delle matrici sorgenti e destinazione 
 * 
 * @param[in]   src1      Prima matrice di input
 * @param[in]   src2      Seconda matrice di input
 * @param[out]  dst       Vettore destinazione
 * @param[in]   height    Dimensione del kernel
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

    T *k = new(std::nothrow) T[height * height];

    if (!k) {
        std::cerr << "Memory allocation failed" <<
        "\n→ Line: " << __LINE__ << 
        "\n→ Function: " << __func__  << 
        "\n→ File: " << __FILE__ << std::endl;;
        exit(EXIT_FAILURE); 
    }

    for (size_t i = 0; i < width - (height - 1); i++) {
        copyElements<T>(src2, k, i, height, width);
        *(dst + i) = argMaxCorr<T>(src1, k, height, width);
    }

    delete k;
}