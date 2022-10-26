#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>

/**
 * Questa funzione scrive i parametri di configurazine della camera nel file: intrinsicExtrinsicParameters.yml, inoltre salva
 * la mappa delle distorsioni per ogni camera nel file: cv::Mat Right_Stereo_Map1.
 * 
 * @param cv::Mat mtxL → matrice degli intrinseci della camera di sinistra.
 * @param cv::Mat distL → vettore dei coefficienti di distorsione della camera di sinistra.
 * @param cv::Mat mtxR → matrice degli intrinseci della camera di destra.
 * @param cv::Mat distR → vettore dei coefficienti di distorsione della camera di destra.
 * @param cv::Mat R → matrice di rotazione.
 * @param cv::Mat T → vettore di traslazione.
 * @param cv::Mat Left_Stereo_Map1 → prima mappa delle distorsioni della camera di sinistra.
 * @param cv::Mat Left_Stereo_Map2 → seconda mappa delle distorsioni della camera di sinistra.
 * @param cv::Mat Right_Stereo_Map1 → prima mappa delle distorsioni della camera di destra.
 * @param cv::Mat Right_Stereo_Map2 → seconda mappa delle distorsioni della camera di destra.
 * 
 * @return void
**/  
void createStereoCameraSetup(cv::Mat mtxL, cv::Mat distL, cv::Mat mtxR,
                             cv::Mat distR, cv::Mat R, cv::Mat T, cv::Mat Left_Stereo_Map1,
                             cv::Mat Left_Stereo_Map2, cv::Mat Right_Stereo_Map1, cv::Mat Right_Stereo_Map2);
