/****************************************************************************
 * Copyright (C) 2022 by Alessio Zattoni                                    *
 *                                                                          *
 * This file is part of calibration.                                        *
 *                                                                          *
 *   calibration is free software: you can redistribute it and/or           *
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
 * @file calibration.hpp
 * @author Alessio Zattoni
 * @date 
 * @brief Questo file contiene la dichiarazione di funzioni per la calibrazione della camera
 *
 * ...
 */



#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <fstream>


/**
 * @brief Questa funzione scrive i parametri di configurazine della camera nel file: \p intrinsicExtrinsicParameters.yml, inoltre
 *        salva la mappa delle distorsioni nel file: \p distortionMapParameters.yml.
 * 
 * @param[in]   mtx     matrice degli intrinseci
 * @param[in]   dist    vettore dei coefficienti di distorsione
 * @param[in]   R       matrice di rotazione
 * @param[in]   T       vettore di traslazione
 * 
 * @return void
**/       
void createStereoCameraSetup(cv::Mat mtx,
                             cv::Mat dist,
                             cv::Mat R,
                             cv::Mat T);


/**
 * @brief Questa funzione crea i parametri di calibrazione per la camera.
 * @note  Se vengono tracciati tutti i corner con diversi colori allora la misura
 *        della checkerboard è corretta e la calibrazione avverrà con successo.
 * 
 * @param[in]   images_path         path dove sono presenti i pattern di calibrazione
 * @param[in]   checkerboard_rows   numero di righe del patter usato in fase di calibrazione, deve essere una scacchiera
 * @param[in]   checkerboard_cols   numero di colonne del patter usato in fase di calibrazione, deve essere una scacchiera
 * 
 * @return void
**/ 
void calibrateSingleCamera(std::string images_path,
                           int         checkerboard_rows, 
                           int         checkerboard_cols);