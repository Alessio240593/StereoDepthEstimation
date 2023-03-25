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
 * @file    benchmark.cu
 * @author  Alessio Zattoni
 * @date 
 * @brief   Questo file contiene il benchmark del progetto di cross-correlazione
 *
 * ...
 */



#include "stereodepth/cross_correlation.hpp"
#include "cuda_cross_correlation.cuh"
#include "test_seq_par.hpp"
#include "analysis.hpp"
#include <fstream>
#include <cstdio>


#define KERNEL_SIZE 3
#define RANGE 50
#define ITERATION 300
#define MIN_SIZE 32
#define MAX_SIZE 1024


void benchmark(int rows, int cols)
{
    uint8_t *src1 = new uint8_t[rows * cols];
    uint8_t *src2 = new uint8_t[rows * cols];
    uint8_t *dest = new uint8_t[(rows - (KERNEL_SIZE - 1)) * (cols - (KERNEL_SIZE - 1))];

    createMatrix<uint8_t>(src1, rows, cols, RANGE);
    createMatrix<uint8_t>(src2, rows, cols, RANGE);

    // benchmark versione sequenziale senza copia
    parco::analysis::TimeVector<double> _seq;

    for (size_t i = 0; i < ITERATION; i++) {
        _seq.start();
        argMaxCorrMat<uint8_t>(src1, src2, dest, cols, rows, KERNEL_SIZE);
        _seq.stop();
    }

    parco::analysis::Matrix<double> matrix_analysis_seq(

            {{"cpu_exec", _seq.values()}});

    std::cout << "=========== MATRIX DATA ============\nrows\t cols\t alg_type\n " << rows << "\t  " << cols << "\t  seq\n";
    matrix_analysis_seq.show_analysis();
    matrix_analysis_seq.dump_analysis_with_size(rows, cols, "seq");

    std::cout << "\n";

    // benchmark versione sequenziale con copia
    parco::analysis::TimeVector<double> _seqC;

    for (size_t i = 0; i < ITERATION; i++) {
        _seqC.start();
        argMaxCorrMatWithCopy<uint8_t>(src1, src2, dest, cols, rows, KERNEL_SIZE);
        _seqC.stop();
    }

    parco::analysis::Matrix<double> matrix_analysis_seqC(

            {{"cpu_exec", _seqC.values()}});

    std::cout << "=========== MATRIX DATA ============\nrows\t cols\t alg_type\n " << rows << "\t  " << cols << "\t  seqC\n";
    matrix_analysis_seqC.show_analysis();
    matrix_analysis_seqC.dump_analysis_with_size(rows, cols, "seqC");

    std::cout << "\n";

    // benchmark versione parallela con tempi di allocazione
    parco::analysis::TimeVector<double> _parA;
    double time;

    for (size_t i = 0; i < ITERATION; i++) {
        time = crossCorrelationTimeWithAllocation(src1, src2, dest, KERNEL_SIZE, rows, cols, rows / 4, cols / 4);
        _parA.values().push_back(time);
    }

    parco::analysis::Matrix<double> matrix_analysis_parA(

            {{"gpu_exec", _parA.values()}});

    std::cout << "=========== MATRIX DATA ============\nrows\t cols\t alg_type\n " << rows << "\t  " << cols << "\t  parA\n";
    matrix_analysis_parA.show_analysis();
    matrix_analysis_parA.dump_analysis_with_size(rows, cols, "parA");

    std::cout << "\n";

    // benchmark versione parallela con tempi di allocazione
    parco::analysis::TimeVector<double> _par;

    for (size_t i = 0; i < ITERATION; i++) {
        time = crossCorrelationTimeWithoutAllocation(src1, src2, dest, KERNEL_SIZE, rows, cols, rows / 4, cols / 4);
        _par.values().push_back(time);
    }

    parco::analysis::Matrix<double> matrix_analysis_par(

            {{"gpu_exec", _par.values()}});

    std::cout << "=========== MATRIX DATA ============\nrows\t cols\t alg_type\n " << rows << "\t  " << cols << "\t  par\n";
    matrix_analysis_par.show_analysis();
    matrix_analysis_par.dump_analysis_with_size(rows, cols, "par");

    delete [] src1; delete [] src2; delete [] dest;
}

namespace fs = std::filesystem;

int main()
{
    if (fs::exists("analysis.csv")) {
        std::remove("analysis.csv");
    }

    for (size_t size = MIN_SIZE; size < MAX_SIZE; size *= 2) {
        benchmark(size, size);
        std::cout << "\n====================================\n\t\tEND\n====================================\n\n";
    }
    
    exit(EXIT_SUCCESS);
}