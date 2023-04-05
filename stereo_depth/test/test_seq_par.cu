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
 * @file    test_seq_par_.cu
 * @author  Alessio Zattoni
 * @date 
 * @brief   Questo file contiene dei test per la versione sequenziale e parallela della cross-correlazione
 *
 * ...
 */



#include "test_seq_par.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>

#include <stdlib.h>

#define MIN_SIZE 32
#define MAX_SIZE 2048
#define RANGE    50
#define INC      2



int main()
{
    // Test vari formati di matrice da MIN_SIZE a MAX_SIZE
    std::size_t kernel_size = 7;
    std::size_t block_dim_x = 8;
    std::size_t block_dim_y = 8;
    for (std::size_t rows_cols = MIN_SIZE; rows_cols <= MAX_SIZE; rows_cols *= INC) {
        launchTest<uint8_t>(rows_cols, rows_cols, kernel_size, block_dim_x, block_dim_y, RANGE);
        kernel_size += INC;
        block_dim_x *= INC;
        block_dim_y *= INC;
    }

    // Test di formati di matrice interessanti per il progetto
    // TODO sistemare misure kernel_size per farlo andare più veloce

    // 1344 x 376
    launchTest<uint8_t>(1344, 376, kernel_size, block_dim_x, block_dim_y, RANGE);
    // 2560 x 720
    launchTest<uint8_t>(2560, 720, kernel_size, block_dim_x, block_dim_y, RANGE);
    // 3840 x 1080
    launchTest<uint8_t>(3840, 1080, kernel_size, block_dim_x, block_dim_y, RANGE);
    // 4416 x 1242
    launchTest<uint8_t>(4416, 1242, kernel_size, block_dim_x, block_dim_y, RANGE);
 
    exit(EXIT_SUCCESS);
}