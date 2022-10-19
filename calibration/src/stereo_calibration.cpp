#include "../include/calibration.hpp"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

//define checkerboard size corners 
#define ROWS 7
#define COLS 10

int main()
{
  cout << "Running stereo calibration ..." << endl;

  // Defining the dimensions of checkerboard
  int CHECKERBOARD[2]{ROWS, COLS}; 

  // Creating vector to store vectors of 3D points for each checkerboard image
  vector<vector<Point3f>> objpoints;

  // Creating vector to store vectors of 2D points for each checkerboard image
  vector<vector<Point2f>> imgpointsL, imgpointsR;

  // Defining the world coordinates for 3D points
  vector<Point3f> objp;
  for(int i{0}; i<CHECKERBOARD[1]; i++) {
    for(int j{0}; j<CHECKERBOARD[0]; j++)
      objp.push_back(Point3f(j,i,0));
  }

  // Extracting path of individual image stored in a given directory
  vector<String> imagesL, imagesR;
  // Path of the folder containing checkerboard images
  string pathL = "../images/left";
  string pathR = "../images/right";

  glob(pathL, imagesL);
  glob(pathR, imagesR);

  Mat frameL, frameR, grayL, grayR;
  // vector to store the pixel coordinates of detected checker board corners 
  vector<Point2f> corner_ptsL, corner_ptsR;
  bool successL, successR;

  // Looping over all the images in the directory
  for(int i{0}; i<imagesL.size(); i++) {
    frameL = imread(imagesL[i]);
    cvtColor(frameL,grayL,COLOR_BGR2GRAY);

    frameR = imread(imagesR[i]);
    cvtColor(frameR,grayR,COLOR_BGR2GRAY);

    // Finding checker board corners
    // If desired number of corners are found in the image then success = true  
    successL = findChessboardCorners(
      grayL,
      Size(CHECKERBOARD[0],CHECKERBOARD[1]),
      corner_ptsL, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

    successR = findChessboardCorners(
      grayR,
      Size(CHECKERBOARD[0],CHECKERBOARD[1]),
      corner_ptsR, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
    /*
      * If desired number of corner are detected,
      * we refine the pixel coordinates and display 
      * them on the images of checker board
    */
    if((successL) && (successR)) {
      TermCriteria criteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);

      // refining pixel coordinates for given 2d points.
      cornerSubPix(grayL,corner_ptsL,Size(11,11), Size(-1,-1),criteria);
      cornerSubPix(grayR,corner_ptsR,Size(11,11), Size(-1,-1),criteria);

      // Displaying the detected corner points on the checker board
      drawChessboardCorners(frameL, Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_ptsL,successL);
      drawChessboardCorners(frameR, Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_ptsR,successR);

      objpoints.push_back(objp);
      imgpointsL.push_back(corner_ptsL);
      imgpointsR.push_back(corner_ptsR);
    }

    imshow("ImageL",frameL);
    moveWindow("ImageL", 0, 0);
    imshow("ImageR",frameR);
    moveWindow("ImageR", 900, 0);

    while (waitKey(1) != 13);
  }

  destroyAllWindows();

  Mat mtxL,distL,R_L,T_L;
  Mat mtxR,distR,R_R,T_R;
  Mat Rot, Trns, Emat, Fmat;
  Mat new_mtxL, new_mtxR;

  // Calibrating left camera
  calibrateCamera(objpoints,
                      imgpointsL,
                      grayL.size(),
                      mtxL,
                      distL,
                      R_L,
                      T_L);

  new_mtxL = getOptimalNewCameraMatrix(mtxL,
                                distL,
                                grayL.size(),
                                1,
                                grayL.size(),
                                0);

  // Calibrating right camera
  calibrateCamera(objpoints,
                      imgpointsR,
                      grayR.size(),
                      mtxR,
                      distR,
                      R_R,
                      T_R);

  new_mtxR = getOptimalNewCameraMatrix(mtxR,
                                distR,
                                grayR.size(),
                                1,
                                grayR.size(),
                                0);

  int flag = 0;
  flag |= CALIB_FIX_ASPECT_RATIO +
          CALIB_USE_INTRINSIC_GUESS +
          CALIB_ZERO_TANGENT_DIST +
          CALIB_SAME_FOCAL_LENGTH,
          TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5);

  // This step is performed to transformation between the two cameras and calculate Essential and 
  // Fundamenatl matrix
  cv::stereoCalibrate(objpoints,
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

  createStereoCameraSetup(new_mtxL, distL, new_mtxR, distR, Rot, Trns);   
  
  cv::Mat rect_l, rect_r, proj_mat_l, proj_mat_r, Q;

  // Once we know the transformation between the two cameras we can perform 
  // stereo rectification
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

  cv::FileStorage cv_file = cv::FileStorage("../calibration_setup/distortion_parameters.xml", cv::FileStorage::WRITE);
  cv_file.write("Left_Stereo_Map_x",Left_Stereo_Map1);
  cv_file.write("Left_Stereo_Map_y",Left_Stereo_Map2);
  cv_file.write("Right_Stereo_Map_x",Right_Stereo_Map1);
  cv_file.write("Right_Stereo_Map_y",Right_Stereo_Map2);

 
  /*
  cv::imshow("Left image before rectification",frameL);
  cv::imshow("Right image before rectification",frameR);


  cv::Mat Left_nice, Right_nice;

  // Apply the calculated maps for rectification and undistortion 
  cv::remap(frameL,
            Left_nice,
            Left_Stereo_Map1,
            Left_Stereo_Map2,
            cv::INTER_LANCZOS4,
            cv::BORDER_CONSTANT,
            0);

  cv::remap(frameR,
            Right_nice,
            Right_Stereo_Map1,
            Right_Stereo_Map2,
            cv::INTER_LANCZOS4,
            cv::BORDER_CONSTANT,
            0);


  cv::imshow("Left image after rectification",Left_nice);
  cv::imshow("Right image after rectification",Right_nice);

  cv::waitKey(0);

  cv::Mat Left_nice_split[3], Right_nice_split[3];

  std::vector<cv::Mat> Anaglyph_channels;

  cv::split(Left_nice, Left_nice_split);
  cv::split(Right_nice, Right_nice_split);

  Anaglyph_channels.push_back(Right_nice_split[0]);
  Anaglyph_channels.push_back(Right_nice_split[1]);
  Anaglyph_channels.push_back(Left_nice_split[2]);

  cv::Mat Anaglyph_img;

  cv::merge(Anaglyph_channels, Anaglyph_img);

  cv::imshow("Anaglyph image", Anaglyph_img);
  cv::waitKey(0);
  */               

  cout << "End of calibratin phase, setup paramaters are in calibration_setup directory." << endl;

  return 0;
}