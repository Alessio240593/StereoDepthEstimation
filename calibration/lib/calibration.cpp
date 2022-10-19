#include <iostream>
#include <fstream>
#include "../include/calibration.hpp"

void createStereoCameraSetup(cv::Mat mtxL, cv::Mat distL, cv::Mat mtxR,
                             cv::Mat distR, cv::Mat R, cv::Mat T)
{
  std::string path = "../calibration_setup/parameters.yml";
  cv::FileStorage fs(path, cv::FileStorage::WRITE);

  if (! fs.isOpened()) { 
    std::cerr << "Error:File did not open, at line " << __LINE__ - 3 << " in file " << __FILE__ << std::endl;
    exit(1); 
  }
  
  //camera parameters
  fs << "CAMERA_MATRIX_LEFT" << mtxL;
  fs << "CAMERA_MATRIX_RIGHT" << mtxR;
  fs << "DISTCOEFFS_RIGHT" << distR;
  fs << "DISTCOEFFS_LEFT" << distL;
  fs << "ROTATION_VECTOR" << R;
  fs << "TRASLATION_VECTOR" << T;
  
  //Close file streams
  fs.release();

  std::cout << "Write Done in file → " << path.substr(path.find_last_of("/") + 1) <<  std::endl;
}