#include <iostream>
#include <fstream>
#include "calibration.hpp"
                 
void createStereoCameraSetup(cv::Mat mtx, cv::Mat dist,
                             cv::Mat R, cv::Mat T)
{
  std::string path = "../calibration_setup/intrinsicExtrinsicParameters.yml";

  cv::FileStorage fs = cv::FileStorage(path, cv::FileStorage::WRITE);

  if (! fs.isOpened()) { 
    std::cerr << "Error:File did not open, at line " << __LINE__ - 3 << " in file " << __FILE__ << std::endl;
    exit(1); 
  }
  
  //camera intrinsic and Extrinsic parameters
  fs << "CAMERA_MATRIX_LEFT" << mtx;
  fs << "DISTCOEFFS_RIGHT" << dist;
  fs << "ROTATION_MATRIX" << R;
  fs << "TRASLATION_VECTOR" << T;

  //Close file streams
  fs.release();

  std::cout << "Write Done in file → " << path.substr(path.find_last_of("/") + 1) <<  std::endl;
}