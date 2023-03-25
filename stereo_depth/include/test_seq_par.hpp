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
                  int           range)
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
                std::cout << "firstImpl → " << +*(firstImpl + (i * cols) + j) << "\n"
                          << "secondImpl → " << +*(secondImpl + (i * cols) + j) << "\n"
                          << "posizione → (" << i << "," << j << ")" << "\n";

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
 * @tparam      T       Tipo di dato utilizzato nell'asserzione
 * 
 * @param[in]   result      Risultato della funzione
 * @param[in]   expected    Valore atteso 
 * 
 *
 * 
 * @return void
*/
template <typename T>
void assertVal(T            result, 
               T            expected, 
               u_int16_t    line)
{
    if (result != expected) {
        std::cerr << "\n\033[1;31m"
                  << "==========ERROR==========="
                  << "\n|| \tLine: " << line << "\t||" 
                  << "\n|| \tCurrent: " << result << "\t||" 
                  << "\n|| \tExpected: " << expected << "\t||"
                  << "\n=========================="
                  << "\033[0m\n";

        exit(EXIT_FAILURE);
    }
}