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
 * @param cv::Mat mtx → matrice degli intrinseci della camera.
 * @param cv::Mat dist → vettore dei coefficienti di distorsione della camera.
 * @param cv::Mat R → matrice di rotazione.
 * @param cv::Mat T → vettore di traslazione.
 * 
 * @return void
**/        
void createStereoCameraSetup(cv::Mat mtx, cv::Mat dist,
                             cv::Mat R, cv::Mat T);