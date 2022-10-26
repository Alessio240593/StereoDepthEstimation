#include "calibration.hpp"
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
  vector<vector<Point2f>> imgpoints;

  // Defining the world coordinates for 3D points
  vector<Point3f> objp;
  for(int i{0}; i<CHECKERBOARD[1]; i++) {
    for(int j{0}; j<CHECKERBOARD[0]; j++)
      objp.push_back(Point3f(j,i,0));
  }

  // Extracting path of individual image stored in a given directory
  vector<String> images;
  // Path of the folder containing checkerboard images
  string path = "../images";

  glob(path, images);

  Mat frame, gray;
  // vector to store the pixel coordinates of detected checker board corners 
  vector<Point2f> corner_pts;
  bool success;

  // Looping over all the images in the directory
  for(int i{0}; i<images.size(); i++) {
    frame = imread(images[i]);
    cvtColor(frame,gray,COLOR_BGR2GRAY);

    // Finding checker board corners
    // If desired number of corners are found in the image then success = true  
    success = findChessboardCorners(
      gray,
      Size(CHECKERBOARD[0],CHECKERBOARD[1]),
      corner_pts, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

    if(success) {
      TermCriteria criteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.001);

      // refining pixel coordinates for given 2d points.
      cornerSubPix(gray,corner_pts,Size(11,11), Size(-1,-1),criteria);

      // Displaying the detected corner points on the checker board
      drawChessboardCorners(frame, Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_pts,success);

      objpoints.push_back(objp);
      imgpoints.push_back(corner_pts);
    }

    imshow("Image",frame);
    moveWindow("Image", 0, 0);

    while (waitKey(1) != 13);
  }

  destroyAllWindows();

  Mat mtx,dist,R,T;
  Mat new_mtx;

  // Calibrating left camera
  double error = calibrateCamera(objpoints,
                      imgpoints,
                      gray.size(),
                      mtx,
                      dist,
                      R,
                      T);

  std::cout << "Reprojection error camera= " << error << "\n";

  new_mtx = getOptimalNewCameraMatrix(mtx,
                                dist,
                                gray.size(),
                                1,
                                gray.size(),
                                0);

  int flag = 0;
  flag |= CALIB_FIX_ASPECT_RATIO +
          CALIB_USE_INTRINSIC_GUESS +
          CALIB_ZERO_TANGENT_DIST +
          CALIB_SAME_FOCAL_LENGTH,
          TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5);
  
  cv::Mat rect, proj_mat, Q;
               
  createStereoCameraSetup(new_mtx, dist, R, T);   

  cv::Mat nice_img;

  std::cout << "Running images distortion retification..." << std::endl;

  for (int file{0}; file < images.size(); file++) {
    frame = imread(images[file]);

    cv::imshow("Image before rectification",frame);
    moveWindow("Image before rectification", 0, 0);

    cv::waitKey(0);

    cv::undistort(frame, nice_img, mtx, dist, new_mtx);

    cv::imshow("Image after rectification",frame);
    moveWindow("Image after rectification", 900, 0);

    cv::waitKey(0);

    destroyAllWindows();
  }

  std::cout << "End of calibratin phase, setup paramaters are in calibration_setup directory." << endl;

  return 0;
}