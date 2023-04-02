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
 * @file    test_seq_par.hpp
 * @author  Alessio Zattoni
 * @date 
 * @brief   Questo file contiene delle funzioni per il test della versione sequenziale e parallela della cross-correlazione
 *
 * ...
 */



#pragma once

#include "stereodepth/cross_correlation.hpp"
#include "cuda_cross_correlation.cuh"
#include "test_seq_par.hpp"
#include "analysis.hpp"
#include <fstream>
#include <cstdio>

// Indica il range di valori con cui verrà riempita la matrice → da 0 a RANGE -1
#define RANGE           50
// Indica il numero di iterazioni eseguite per ogni implementazione dell'algoritmo di cross correlazione
#define ITERATION       100

// Rappresenta una matrice in formato standard es(HD, 2k, 4K)
namespace format {
    class standardFormat{
        private:
            const std::size_t rows;
            const std::size_t cols;
        
        public:
            standardFormat(int rows, int cols) : cols(cols), rows(rows) {};

            inline const std::size_t getRows() const
            {
                return rows;
            }

            inline std::size_t getCols() const
            {
                return cols;
            }

            friend std::ostream& operator << (std::ostream& os, const format::standardFormat& format)
            {
                os << "\nRows: " << format.rows
                   << "\nCols: " << format.cols << std::endl;
                return os;
            }
    };
}

// Rappresenta dei formati standard utilizzati in fase di benchmark, se presente un nuovo formato basta aggiungerlo qui
const static std::vector<format::standardFormat> form = {   {1344, 376}, 
                                                            {2560, 720},
                                                            {3840, 1080},
                                                            {4416, 1242}    };



/**
 * @brief   Questa funzione esegue il benchmark della matrice \p rows x \p cols, con un kernel di dimensione \p kernel_size
 *          e blocchi di thread della dimensione \p block_dim_x x \p block_dim_y
 * @note    La funzione stampa a video il risultato del benchmark con media, deviazione standard e mediana dei tempi, inoltre
 *          salva i risultati in un file con estensione csv così da averli disponibili per una successiva elaborazione. Per 
 *          calcolare lo speedup dei tempi basta eseguire il programma dentro la cartella python denominato appunto \p speed_up.py
 * 
 * @param[in]   rows        Numero di righe della matrice
 * @param[in]   cols        Numero di colonne della matrice
 * @param[in]   kernel_size Dimensione del kernel
 * @param[in]   block_dim_x Dimensione del blocco di thread lungo la coordinata x
 * @param[in]   block_dim_y Dimensione del blocco di thread lungo la coordinata y
 * 
 * 
 * @return void
*/
template <typename T>
void benchmark(std::size_t  rows, 
               std::size_t  cols, 
               std::size_t  kernel_size, 
               std::size_t  block_dim_x,
               std::size_t  block_dim_y)
{
    T *src1 = new T[rows * cols];
    T *src2 = new T[rows * cols];
    T *dest = new T[(rows - (kernel_size - 1)) * (cols - (kernel_size - 1))];

    createMatrix<T>(src1, rows, cols, RANGE);
    createMatrix<T>(src2, rows, cols, RANGE);

    // benchmark versione sequenziale senza copia
    parco::analysis::TimeVector<double> _seq;

    for (size_t i = 0; i < ITERATION; i++) {
        _seq.start();
        argMaxCorrMat<T>(src1, src2, dest, cols, rows, kernel_size);
        _seq.stop();
    }

    // benchmark versione sequenziale con copia
    parco::analysis::TimeVector<double> _seq_c;

    for (size_t i = 0; i < ITERATION; i++) {
        _seq_c.start();
        argMaxCorrMatWithCopy<T>(src1, src2, dest, cols, rows, kernel_size);
        _seq_c.stop();
    }

    // benchmark versione parallela con tempi di allocazione
    parco::analysis::TimeVector<double> _par;
    double time;

    for (size_t i = 0; i < ITERATION; i++) {
        time = crossCorrelationTimeWithoutAllocation<T>(src1, src2, dest, kernel_size, rows, cols, block_dim_x, block_dim_y);
        _par.values().push_back(time);
    }

    // benchmark versione parallela con tempi di allocazione
    parco::analysis::TimeVector<double> _par_c;

    for (size_t i = 0; i < ITERATION; i++) {
        time = crossCorrelationTimeWithAllocation<T>(src1, src2, dest, kernel_size, rows, cols, block_dim_x, block_dim_y);
        _par_c.values().push_back(time);
    }

    parco::analysis::Matrix<double> matrix_analysis(
            {
                {"cpu_seq_exec", _seq.values()},
                {"cpu_seq_c_exec", _seq_c.values()},
                {"gpu_par_exec", _par.values()},
                {"gpu_par_c_exec", _par_c.values()}
            }, "matrix: " + std::to_string(rows) + "x" + std::to_string(cols) +
               ",kernel: " + std::to_string(kernel_size) + "x" + std::to_string(kernel_size) +
               ",BlockDim: " + std::to_string(block_dim_x) + "x" + std::to_string(block_dim_y));

    matrix_analysis.show_analysis();
    matrix_analysis.dump_analysis("results");

    delete [] src1; delete [] src2; delete [] dest;
}