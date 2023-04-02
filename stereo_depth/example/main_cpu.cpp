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
 * @file    main_cpu.cpp
 * @author  Alessio Zattoni
 * @date 
 * @brief   Questo file contiene un esempio di calcolo di cross-correlazione
 *
 * ...
 */



#include <stereodepth/cross_correlation.hpp>
#include <iostream>
#include <vector>
#include <cstring>

/// Dimensione del kernel
#define KERNEL_SIZE 3
/// Larghezza della matrice sorgente
#define WIDTH       10
/// Altezza della matrice sorgente
#define HEIGHT      3
/// Incremento delle matrici sorgenti (src3 e src4)
#define INC         2

int main()
{
    const std::vector<uint8_t> src1{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    const std::vector<uint8_t> src2{
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 1, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0
    };

    const std::vector<uint8_t> src3{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    };

    const std::vector<uint8_t> src4{
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 1, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    std::vector<uint8_t> dst_matrix((WIDTH - (KERNEL_SIZE - 1)) * ((HEIGHT + INC) - (KERNEL_SIZE - 1)), 0);
    std::vector<uint8_t> dst_vector(WIDTH - (KERNEL_SIZE - 1), 0);
    std::vector<uint8_t> dst_padding((HEIGHT + (KERNEL_SIZE - 1)) * (WIDTH + (KERNEL_SIZE - 1)), 0);

    /*std::vector<uint8_t> k{
        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    };*/
    argMaxCorrVector<uint8_t>(src1.data(), src2.data(), dst_vector.data(), HEIGHT, WIDTH);
    argMaxCorrMat<uint8_t>(src3.data(), src4.data(), dst_matrix.data(), WIDTH, (HEIGHT + INC), KERNEL_SIZE);

    // Stampo la matrice src1
    const uint8_t *pSrc = src1.data();
    std::cout << "\n→ src1:\n";
    for (std::size_t i = 0; i < HEIGHT; i++) {
        for (std::size_t j = 0; j < WIDTH; j++) {
            std::cout << +*(pSrc +(i * WIDTH) + j) << " ";
        }
        std::cout <<"\n";
    }

    // Stampo la matrice src2
    pSrc = src2.data();
    std::cout << "\n→ src2:\n";
    for (std::size_t i = 0; i < HEIGHT; i++) {
        for (std::size_t j = 0; j < WIDTH; j++) {
            std::cout << +*(pSrc +(i * WIDTH) + j) << " ";
        }
        std::cout <<"\n";
    }

    // Stampo risultato argMaxCorrVector
    pSrc = dst_vector.data();
    std::cout << "\n→ Risultato argMaxCorrVector src1, src2: ";

    for (std::size_t j = 0; j < (WIDTH - (KERNEL_SIZE - 1)); j++) {
        std::cout << +*(pSrc + j) << " ";
    }

    // Stampo risultato del pagging
    std::cout <<"\n\n→ Risultato padding src2:\n";
    padding<uint8_t>(src2.data(), dst_padding.data(), HEIGHT, WIDTH, KERNEL_SIZE, KERNEL_SIZE);
    pSrc = dst_padding.data();

    for (std::size_t i = 0; i < (HEIGHT + (KERNEL_SIZE - 1)); i++) {
        for (std::size_t j = 0; j < (WIDTH + (KERNEL_SIZE - 1)); j++) {
            std::cout << +*(pSrc +(i * (WIDTH + (KERNEL_SIZE - 1)) + j)) << " ";
        }
        std::cout <<"\n";
    }

    // Stampo la matrice src3
    pSrc = src3.data();
    std::cout << "\n→ src3:\n";
    for (std::size_t i = 0; i < HEIGHT + INC; i++) {
        for (std::size_t j = 0; j < WIDTH; j++) {
            std::cout << +*(pSrc +(i * WIDTH) + j) << " ";
        }
        std::cout <<"\n";
    }

    // Stampo la matrice src4
    pSrc = src4.data();
    std::cout << "\n→ src4:\n";
    for (std::size_t i = 0; i < HEIGHT + INC; i++) {
        for (std::size_t j = 0; j < WIDTH; j++) {
            std::cout << +*(pSrc +(i * WIDTH) + j) << " ";
        }
        std::cout <<"\n";
    }

    // Stampo il risultato della cross correlazione
    pSrc = dst_matrix.data();
    std::cout <<"\n\n→ Risultato argMaxCorrMat src2, src3:\n";
    for (std::size_t i = 0; i < (HEIGHT + INC) - (KERNEL_SIZE - 1); i++) {
        for (std::size_t j = 0; j < WIDTH - (KERNEL_SIZE - 1); j++) {
            std::cout << +*(pSrc + j + (i * (WIDTH - (KERNEL_SIZE - 1)))) << " ";
        }
        std::cout << std::endl;
    }
    
    exit(EXIT_SUCCESS);
}