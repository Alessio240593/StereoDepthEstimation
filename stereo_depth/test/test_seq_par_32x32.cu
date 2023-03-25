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
 * @file    test_seq_par_32x32.cu
 * @author  Alessio Zattoni
 * @date 
 * @brief   Questo file contiene dei test per la versione sequenziale e parallela della cross-correlazione con matrici 32x32
 *
 * ...
 */



#include "test_seq_par.hpp"
#include "stereodepth/cross_correlation.hpp"
#include "cuda_cross_correlation.cuh"
#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>

#include <stdlib.h>

#define KERNEL_SIZE 5
#define SRC_ROWS 32
#define SRC_COLS 32
#define DEST_ROWS (SRC_ROWS - (KERNEL_SIZE - 1))
#define DEST_COLS (SRC_COLS - (KERNEL_SIZE - 1))
#define RANGE 50


int main()
{
    uint8_t *src1 = new uint8_t[SRC_ROWS * SRC_COLS];
    uint8_t *src2 = new uint8_t[SRC_ROWS * SRC_COLS];
    uint8_t *seqDest = new uint8_t[DEST_ROWS * DEST_COLS];
    uint8_t *parDest = new uint8_t[DEST_ROWS * DEST_COLS];
    uint8_t *seqDestCopy = new uint8_t[DEST_ROWS * DEST_COLS];

    createMatrix<uint8_t>(src1, SRC_ROWS, SRC_COLS, RANGE);
    createMatrix<uint8_t>(src2, SRC_ROWS, SRC_COLS, RANGE);

    /*std::cout << "Src1 → \n";
    for (std::size_t i = 0; i < ROWS; i++) {
        for (std::size_t j = 0; j < COLS; j++) {
            std::cout << std::right << std::setw(3) <<  +*(src1 + (i * COLS) + j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n\n";

    std::cout << "Src2 → \n";
    for (std::size_t i = 0; i < ROWS; i++) {
        for (std::size_t j = 0; j < COLS; j++) {
            std::cout << std::right << std::setw(3) << +*(src2 + (i * COLS) + j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n\n";*/

    argMaxCorrMat<uint8_t>(src1, src2, seqDest, SRC_COLS, SRC_ROWS, KERNEL_SIZE);

    argMaxCorrMatWithCopy<uint8_t>(src1, src2, seqDestCopy, SRC_COLS, SRC_ROWS, KERNEL_SIZE);

    crossCorrelation<uint8_t>(src1, src2, parDest, KERNEL_SIZE, SRC_ROWS, SRC_COLS);

    bool res = checkAlgorithmConsistency(seqDest, parDest, DEST_ROWS, DEST_COLS);
    assertVal<bool>(res, true, __LINE__);

    res = checkAlgorithmConsistency(seqDest, seqDestCopy, DEST_ROWS, DEST_COLS);
    assertVal<bool>(res, true, __LINE__);

    res = checkAlgorithmConsistency(seqDestCopy, parDest, DEST_ROWS, DEST_COLS);
    assertVal<bool>(res, true, __LINE__);

    /*
    std::cout << "seqDest → \n";
    for (std::size_t i = 0; i < DEST_ROWS; i++) {
        for (std::size_t j = 0; j < DEST_COLS; j++) {
            std::cout << std::right << std::setw(3) <<  +*(seqDest + (i * DEST_COLS) + j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n\n";

     std::cout << "parDest → \n";
    for (std::size_t i = 0; i < DEST_ROWS; i++) {
        for (std::size_t j = 0; j < DEST_COLS; j++) {
            std::cout << std::right << std::setw(3) <<  +*(parDest + (i * DEST_COLS) + j) << " ";
        }
        std::cout << std::endl;
    }
    */

    delete [] src1; delete [] src2; delete [] seqDest; delete [] parDest; delete [] seqDestCopy;  

    exit(EXIT_SUCCESS);
}