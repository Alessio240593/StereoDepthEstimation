/****************************************************************************
 * Copyright (C) 2022 by Alessio Zattoni                                    *
 *                                                                          *
 * This file is part of stereo_calibration.                                 *
 *                                                                          *
 *   stereo_calibration is free software: you can redistribute it and/or    *
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
 * @file calibration.cpp
 * @author Alessio Zattoni
 * @date 
 * @brief Questo file contiene l'implementazione di funzioni per la calibrazione della camera stereo
 *
 * ...
 */



#include "calibration.hpp"


void createStereoCameraSetup(cv::Mat mtxL,
                             cv::Mat distL,
                             cv::Mat mtxR,
                             cv::Mat distR,
                             cv::Mat R,
                             cv::Mat T,
                             cv::Mat Left_Stereo_Map1,
                             cv::Mat Left_Stereo_Map2,
                             cv::Mat Right_Stereo_Map1,
                             cv::Mat Right_Stereo_Map2)
{
  std::string pathToIE = "../calibration_setup/intrinsicExtrinsicParameters.yml";
  std::string pathTOMap = "../calibration_setup/distortionMapParameters.yml";

  cv::FileStorage fsIE = cv::FileStorage(pathToIE, cv::FileStorage::WRITE);
  cv::FileStorage fsMap = cv::FileStorage(pathTOMap, cv::FileStorage::WRITE);

  if (! fsIE.isOpened()) { 
    std::cerr << "Error:File did not open, at line " << __LINE__ - 4 << " in file " << __FILE__ << std::endl;
    exit(1); 
  }

  if (! fsMap.isOpened())
  {
    std::cerr << "Error:File did not open, at line " << __LINE__ - 3 << " in file " << __FILE__ << std::endl;
    exit(1); 
  }
  
  //camera intrinsic and Extrinsic parameters
  fsIE << "CAMERA_MATRIX_LEFT" << mtxL;
  fsIE << "CAMERA_MATRIX_RIGHT" << mtxR;
  fsIE << "DISTCOEFFS_RIGHT" << distR;
  fsIE << "DISTCOEFFS_LEFT" << distL;
  fsIE << "ROTATION_MATRIX" << R;
  fsIE << "TRASLATION_VECTOR" << T;

  // distortion maps
  fsMap << "LEFT_STEREO_MAP_X" << Left_Stereo_Map1;
  fsMap << "LEFT_STEREO_MAP_Y" << Left_Stereo_Map2;
  fsMap << "RIGHT_STEREO_MAP_X" << Right_Stereo_Map1;
  fsMap << "RIGHT_STEREO_MAP_Y" << Right_Stereo_Map2;
  
  //Close file streams
  fsIE.release();
  fsMap.release();

  std::cout << "Write Done in file → " << pathToIE.substr(pathToIE.find_last_of("/") + 1) <<  std::endl;
  std::cout << "Write Done in file → " << pathTOMap.substr(pathTOMap.find_last_of("/") + 1) <<  std::endl;
}


void calibrateStereoCamera(std::string left_images_path,
                           std::string right_images_path, 
                           int         checkerboard_rows, 
                           int         checkerboard_cols)
{
  std::cout << "Running stereo calibration ..." << std::endl;

  // Defining the dimensions of checkerboard
  int CHECKERBOARD[2]{checkerboard_rows, checkerboard_cols}; 

  // Creating vector to store vectors of 3D points for each checkerboard image
  std::vector<std::vector<cv::Point3f>> objpoints;

  // Creating vector to store vectors of 2D points for each checkerboard image
  std::vector<std::vector<cv::Point2f>> imgpointsL, imgpointsR;

  // Defining the world coordinates for 3D points
  std::vector<cv::Point3f> objp;
  for(int i{0}; i<CHECKERBOARD[1]; i++) {
    for(int j{0}; j<CHECKERBOARD[0]; j++)
      objp.push_back(cv::Point3f(j,i,0));
  }

  // Extracting path of individual image stored in a given directory
  std::vector<cv::String> imagesL, imagesR;
  // Path of the folder containing checkerboard images
  std::string pathL = left_images_path;
  std::string pathR = right_images_path;

  cv::glob(pathL, imagesL);
  cv::glob(pathR, imagesR);

  cv::Mat frameL, frameR, grayL, grayR;
  // vector to store the pixel coordinates of detected checker board corners 
  std::vector<cv::Point2f> corner_ptsL, corner_ptsR;
  bool successL, successR;

  // Looping over all the images in the directory
  for(int i{0}; i<imagesL.size(); i++) {
    frameL = cv::imread(imagesL[i]);
    cv::cvtColor(frameL,grayL,cv::COLOR_BGR2GRAY);

    frameR = cv::imread(imagesR[i]);
    cv::cvtColor(frameR,grayR,cv::COLOR_BGR2GRAY);

    // Finding checker board corners
    // If desired number of corners are found in the image then success = true  
    successL = cv::findChessboardCorners(
      grayL,
      cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]),
      corner_ptsL, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

    successR = findChessboardCorners(
      grayR,
      cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]),
      corner_ptsR, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
    /*
      * If desired number of corner are detected,
      * we refine the pixel coordinates and display 
      * them on the images of checker board
    */
    if((successL) && (successR)) {
      cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

      // refining pixel coordinates for given 2d points.
      cv::cornerSubPix(grayL,corner_ptsL,cv::Size(11,11), cv::Size(-1,-1),criteria);
      cv::cornerSubPix(grayR,corner_ptsR,cv::Size(11,11), cv::Size(-1,-1),criteria);

      // Displaying the detected corner points on the checker board
      cv::drawChessboardCorners(frameL, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_ptsL,successL);
      cv::drawChessboardCorners(frameR, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_ptsR,successR);

      objpoints.push_back(objp);
      imgpointsL.push_back(corner_ptsL);
      imgpointsR.push_back(corner_ptsR);
    }

    cv::imshow("ImageL",frameL);
    cv::moveWindow("ImageL", 0, 0);
    cv::imshow("ImageR",frameR);
    cv::moveWindow("ImageR", 900, 0);

    while (cv::waitKey(1) != 13);
  }

  cv::destroyAllWindows();

  cv::Mat mtxL,distL,R_L,T_L;
  cv::Mat mtxR,distR,R_R,T_R;
  cv::Mat Rot, Trns, Emat, Fmat;
  cv::Mat new_mtxL, new_mtxR;

  // Calibrating left camera
  double error = cv::calibrateCamera(objpoints,
                      imgpointsL,
                      grayL.size(),
                      mtxL,
                      distL,
                      R_L,
                      T_L);

  std::cout << "Reprojection error left camera= " << error << "\n";

  new_mtxL = cv::getOptimalNewCameraMatrix(mtxL,
                                distL,
                                grayL.size(),
                                1,
                                grayL.size(),
                                0);

  // Calibrating right camera
  error = cv::calibrateCamera(objpoints,
                      imgpointsR,
                      grayR.size(),
                      mtxR,
                      distR,
                      R_R,
                      T_R);

  std::cout << "Reprojection error right camera= " << error << "\n";

  /*
  for (auto i = 0; i < T_R.cols + 2; i++)
  {
    for (auto j = 0; j < T_R.rows; j++)
    {
      T_R.at<double>(i, j) = T_R.at<double>(i, j) * 2.4;
    }
  }

  cout << T_R.row(0) << endl;
  */

  new_mtxR = cv::getOptimalNewCameraMatrix(mtxR,
                                distR,
                                grayR.size(),
                                1,
                                grayR.size(),
                                0);

  int flag = 0;
  flag |= cv::CALIB_FIX_ASPECT_RATIO +
          cv::CALIB_USE_INTRINSIC_GUESS +
          cv::CALIB_ZERO_TANGENT_DIST +
          cv::CALIB_SAME_FOCAL_LENGTH,
          cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-5);

  // This step is performed to transformation between the two cameras and calculate Essential and 
  // Fundamenatl matrix
  //  This function finds the intrinsic parameters for each of the two cameras and the extrinsic parameters between the two cameras.
  error = cv::stereoCalibrate(objpoints,
                      imgpointsL,
                      imgpointsR,
                      new_mtxL,
                      distL,
                      new_mtxR,
                      distR,
                      grayR.size(),
                      Rot,
                      Trns,
                      Emat,
                      Fmat,
                      flag,
                      cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 1e-6));
  
  std::cout << "Reprojection error = " << error << "\n";

  /*
  cout << Trns <<endl;
    for (auto j = 0; j < Trns.rows; j++)
    {
      Trns.at<double>(j, 0) = Trns.at<double>(j, 0) * 2.4;
    }

  cout << Trns <<endl;
  */
  
  cv::Mat rect_l, rect_r, proj_mat_l, proj_mat_r, Q;

  // Once we know the transformation between the two cameras we can perform 
  // stereo rectification
  // retifica l'asse y così da far coincidere l'origine infatti così facendo la scanline è solo lungo l'asse x poichè la y coincide
  cv::stereoRectify(new_mtxL,
                    distL,
                    new_mtxR,
                    distR,
                    grayR.size(),
                    Rot,
                    Trns,
                    rect_l,
                    rect_r,
                    proj_mat_l,
                    proj_mat_r,
                    Q,
                    1);

  cv::Mat Left_Stereo_Map1, Left_Stereo_Map2;
  cv::Mat Right_Stereo_Map1, Right_Stereo_Map2;
  
  cv::initUndistortRectifyMap(new_mtxL,
                              distL,
                              rect_l,
                              proj_mat_l,
                              grayR.size(),
                              CV_16SC2,
                              Left_Stereo_Map1,
                              Left_Stereo_Map2);
                              

  cv::initUndistortRectifyMap(new_mtxR,
                              distR,
                              rect_r,
                              proj_mat_r,
                              grayR.size(),
                              CV_16SC2,
                              Right_Stereo_Map1,
                              Right_Stereo_Map2);

  createStereoCameraSetup(new_mtxL, distL, new_mtxR, distR, Rot, Trns, Left_Stereo_Map1, Left_Stereo_Map2, Right_Stereo_Map1, Right_Stereo_Map2);   

  cv::Mat Left_nice, Right_nice;

  std::cout << "Running left images distortion retification..." << std::endl;

  for (int file{0}; file < imagesL.size(); file++) {
    frameL = cv::imread(imagesL[file]);

    cv::imshow("Left image before rectification",frameL);
    cv::moveWindow("Left image before rectification", 0, 0);

    cv::waitKey(0);

    cv::remap(frameL,
            Left_nice,
            Left_Stereo_Map1,
            Left_Stereo_Map2,
            cv::INTER_LANCZOS4,
            cv::BORDER_CONSTANT,
            0);

    cv::imshow("Left image after rectification",frameL);
    cv::moveWindow("Left image after rectification", 900, 0);

    cv::waitKey(0);

    cv::destroyAllWindows();
  }

  std::cout << "Running right images distortion retification" << std::endl;

  for (int file{0}; file < imagesR.size(); file++) {
    frameR = cv::imread(imagesR[file]);

    cv::imshow("Right image before rectification",frameR);
    cv::moveWindow("Right image before rectification", 0, 0);

    cv::waitKey(0);

    cv::remap(frameR,
            Right_nice,
            Right_Stereo_Map1,
            Right_Stereo_Map2,
            cv::INTER_LANCZOS4,
            cv::BORDER_CONSTANT,
            0);

      cv::imshow("Right image after rectification",frameR);
      cv::moveWindow("Right image after rectification", 900, 0);

      cv::waitKey(0);

      cv::destroyAllWindows();
  }

  std::cout << "End of calibratin phase, setup paramaters are in calibration_setup directory." << std::endl;
}