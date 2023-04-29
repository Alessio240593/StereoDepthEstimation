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

#include <cstddef>
#include <iostream>
#include "stereodepth/cross_correlation.hpp"
#include "cuda_cross_correlation.cuh"



/**
 * @brief   Questa funzione riempie la matrice \p rows x \p cols con valori randomici da 0 a \p range - 1
 * 
 * @tparam      T       Tipo della matrice sorgente 
 * 
 * @param[out]  src     Matrice sorgente
 * @param[in]   rows    Numero di righe delle due matrici
 * @param[in]   cols    Numero di colonne delle due matrici
 * @param[in]   range   Specifica il range di produzione dei valori randomici, parte da 0 fino a \p range - 1
 * 
 *
 * 
 * @return  void
*/
template <typename T>
void createMatrix(T             *src, 
                  std::size_t   rows, 
                  std::size_t   cols, 
                  std::size_t   range)
{
    std::srand(time(NULL));

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            *(src + (i * cols) + j) = rand() % range;
        }
    }
}


/**
 * @brief   Questa funzione serve per controllare che le due implementazioni calcolino la stessa funzione (cross-correlazione)
 * @note    Se esiste, la funzione stampa la posizione e i rispettivi valori del primo valore che trova diverso
 *          nelle due matrici
 * 
 * @tparam      T           Tipo delle matrici sorgenti 
 * 
 * @param[in]   firstImpl   Matrice di cross-correlazione della prima implementazione dell'algoritmo di cross-correlazione
 * @param[in]   secondImpl  Matrice di cross-correlazione della seconda implementazione dell'algoritmo di cross-correlazione
 * @param[in]   rows        Numero di righe delle due matrici
 * @param[in]   cols        Numero di colonne delle due matrici
 * 
 *
 * 
 * @return  bool
 * 
 * @retval  true    Se tutti i valori nelle relative posizioni sono uguali
 * @retval  false   Se almeno un valore della matrice \p seqSrc è diverso dal corrispettivo valore nella matrice \p parSrc
*/
template <typename T>
bool checkAlgorithmConsistency(T            *firstImpl, 
                               T            *secondImpl, 
                               std::size_t  rows, 
                               std::size_t  cols)
{
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            if (*(firstImpl + (i * cols) + j) != *(secondImpl + (i * cols) + j)) {
                std::cout << "\n\nseq:\n";
            for (size_t k = 0; k < rows; k++) {
                for (size_t l = 0; l < cols; l++) {
                    std::cout << +*(firstImpl + (k * rows) + l) << " ";
                }
                std::cout << std::endl;
            }

            std::cout << "shmem:\n";
            for (size_t k = 0; k < rows; k++) {
                for (size_t l = 0; l < cols; l++) {
                    std::cout << +*(secondImpl + (k * rows) + l) << " ";
                }
                std::cout << std::endl;
            }
                // std::cout << "firstImpl → " << +*(firstImpl + (i * cols) + j) << "\n"
                //           << "secondImpl → " << +*(secondImpl + (i * cols) + j) << "\n"
                //           << "posizione → (" << i << "," << j << ")" << "\n";

                return false;
            }
        }
    }
    return true;
}


/**
 * @brief   Questa funzione testa il risultato di una procedura secondo un'asserzione
 * @note    Se l'asserzione non è rispettata la funzione stampa un messaggio di errore
 *          indicando il valore corrente e il valore atteso
 * 
 * @tparam      T           Tipo di dato utilizzato nell'asserzione
 * 
 * @param[in]   result      Risultato della funzione
 * @param[in]   expected    Valore atteso 
 * @param[in]   rows        Numero di righe della matrice usata per il test
 * @param[in]   cols        Numero di colonne della matrice usata per il test
 * @param[in]   line        Posizione della chiamata \p assertVal
 * 
 *
 * 
 * @return void
*/
template <typename T>
void assertVal(T            result, 
               T            expected,
               std::size_t  rows,
               std::size_t  cols,
               u_int16_t    line)
{
    if (result != expected) {
        std::cerr << "\n\033[1;31m"
                  << "===========ERROR============"
                  << "\n \tLine: " << line
                  << "\n \tSize: " << rows << "x" << cols
                  << "\n \tCurrent: " << result 
                  << "\n \tExpected: " << expected
                  << "\n============================"
                  << "\033[0m\n";

        exit(EXIT_FAILURE);
    }
}


/**
 * @brief   Questa funzione lancia il test di consistenza nel calcolo tra le varie versioni,
 *          per matrici di dimensione \p rows x \p cols
 * @note    Il parametro range definisce i valori con cui verranno riempite le matrici in modo
 *          randomico, l'intervallo parte da 0 fino a \p range - 1
 * 
 * @tparam      T           Tipo delle matrici utilizzate nel calcolo della cross correlazione
 * 
 * @param[in]   src_rows    Numero di righe della matrice usata per il test
 * @param[in]   src_cols    Numero di colonne della matrice usata per il test 
 * @param[in]   kernel_size Misura del kernel usato per il test
 * @param[in]   block_dim_x Dimensione del blocco di thread nella coordinata x
 * @param[in]   block_dim_y Dimensione del blocco di thread nella coordinata y
 * @param[in]   range       Range di valori con cui verranno riempite le matrici per il test
 * 
 *
 * 
 * @return void
*/
template <typename T>
void launchTest(std::size_t src_rows,
                std::size_t src_cols,
                std::size_t kernel_size,
                std::size_t block_dim_x,
                std::size_t block_dim_y,
                std::size_t range)
{
    std::size_t dst_rows = src_rows - (kernel_size - 1);
    std::size_t dst_cols = src_cols - (kernel_size - 1);

    T *src1                = new T[src_rows * src_cols];
    T *src2                = new T[src_rows * src_cols];
    T *seq_dest            = new T[dst_rows * dst_cols];
    T *par_dest            = new T[dst_rows * dst_cols];
    T *seq_dest_copy       = new T[dst_rows * dst_cols];
    T *par_dest_shared     = new T[dst_rows * dst_cols];

    createMatrix<T>(src1, src_rows, src_cols, range);
    createMatrix<T>(src2, src_rows, src_cols, range);

    argMaxCorrMat<T>(src1, src2, seq_dest, src_cols, src_rows, kernel_size);

    argMaxCorrMatWithCopy<T>(src1, src2, seq_dest_copy, src_cols, src_rows, kernel_size);

    crossCorrelation<T>(src1, src2, par_dest, kernel_size, src_rows, src_cols, block_dim_x, block_dim_y);

    sharedMemoryCrossCorrelation<T>(src1, src2, par_dest_shared, kernel_size, src_rows, src_cols, block_dim_x, block_dim_y);

    bool res = checkAlgorithmConsistency(seq_dest, par_dest, dst_rows, dst_cols);
    assertVal<bool>(res, true, src_rows, src_cols, __LINE__);

    res = checkAlgorithmConsistency(seq_dest, seq_dest_copy, dst_rows, dst_cols);
    assertVal<bool>(res, true, src_rows, src_cols, __LINE__);

    res = checkAlgorithmConsistency(seq_dest, par_dest_shared, dst_rows, dst_cols);
    assertVal<bool>(res, true, src_rows, src_cols, __LINE__);

    res = checkAlgorithmConsistency(seq_dest_copy, par_dest, dst_rows, dst_cols);
    assertVal<bool>(res, true, src_rows, src_cols, __LINE__);

    res = checkAlgorithmConsistency(seq_dest_copy, par_dest_shared, dst_rows, dst_cols);
    assertVal<bool>(res, true, src_rows, src_cols, __LINE__);

    res = checkAlgorithmConsistency(par_dest, par_dest_shared, dst_rows, dst_cols);
    assertVal<bool>(res, true, src_rows, src_cols, __LINE__);

    delete [] src1; delete [] src2; delete [] seq_dest; delete [] par_dest; delete [] seq_dest_copy; delete [] par_dest_shared;
}