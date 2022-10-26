#include <iostream>
#include <fstream>
#include "../include/calibration.hpp"
                       
void createStereoCameraSetup(cv::Mat mtxL, cv::Mat distL, cv::Mat mtxR,
                             cv::Mat distR, cv::Mat R, cv::Mat T, cv::Mat Left_Stereo_Map1,
                             cv::Mat Left_Stereo_Map2, cv::Mat Right_Stereo_Map1, cv::Mat Right_Stereo_Map2)
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