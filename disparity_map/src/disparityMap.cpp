#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/core.hpp>

using namespace cv;

// initialize values for StereoSGBM parameters
int numDisparities = 8;
int blockSize = 5;
int preFilterType = 1;
int preFilterSize = 1;
int preFilterCap = 31;
int minDisparity = 0;
int textureThreshold = 10;
int uniquenessRatio = 15;
int speckleRange = 0;
int speckleWindowSize = 0;
int disp12MaxDiff = -1;
int dispType = CV_16S;

// Creating an object of StereoSGBM algorithm
cv::Ptr<cv::StereoBM> stereo = cv::StereoBM::create();

cv::Mat imgL;
cv::Mat imgR;
cv::Mat imgL_gray;
cv::Mat imgR_gray;

// Defining callback functions for the trackbars to update parameter values

static void on_trackbar1( int, void* )
{
  stereo->setNumDisparities(numDisparities*16);
  numDisparities = numDisparities*16;
}

static void on_trackbar2( int, void* )
{
  stereo->setBlockSize(blockSize*2+5);
  blockSize = blockSize*2+5;
}

static void on_trackbar3( int, void* )
{
  stereo->setPreFilterType(preFilterType);
}

static void on_trackbar4( int, void* )
{
  stereo->setPreFilterSize(preFilterSize*2+5);
  preFilterSize = preFilterSize*2+5;
}

static void on_trackbar5( int, void* )
{
  stereo->setPreFilterCap(preFilterCap);
}

static void on_trackbar6( int, void* )
{
  stereo->setTextureThreshold(textureThreshold);
}

static void on_trackbar7( int, void* )
{
  stereo->setUniquenessRatio(uniquenessRatio);
}

static void on_trackbar8( int, void* )
{
  stereo->setSpeckleRange(speckleRange);
}

static void on_trackbar9( int, void* )
{
  stereo->setSpeckleWindowSize(speckleWindowSize*2);
  speckleWindowSize = speckleWindowSize*2;
}

static void on_trackbar10( int, void* )
{
  stereo->setDisp12MaxDiff(disp12MaxDiff);
}

static void on_trackbar11( int, void* )
{
  stereo->setMinDisparity(minDisparity);
}

using namespace std;
int main()
{
 /* // Creating a named window to be linked to the trackbars
  cv::namedWindow("disparity",cv::WINDOW_NORMAL);
  cv::resizeWindow("disparity",900,900);

  // Creating trackbars to dynamically update the StereoBM parameters
  cv::createTrackbar("numDisparities", "disparity", &numDisparities, 18, on_trackbar1);
  cv::createTrackbar("blockSize", "disparity", &blockSize, 50, on_trackbar2);
  cv::createTrackbar("preFilterType", "disparity", &preFilterType, 1, on_trackbar3);
  cv::createTrackbar("preFilterSize", "disparity", &preFilterSize, 25, on_trackbar4);
  cv::createTrackbar("preFilterCap", "disparity", &preFilterCap, 62, on_trackbar5);
  cv::createTrackbar("textureThreshold", "disparity", &textureThreshold, 100, on_trackbar6);
  cv::createTrackbar("uniquenessRatio", "disparity", &uniquenessRatio, 100, on_trackbar7);
  cv::createTrackbar("speckleRange", "disparity", &speckleRange, 100, on_trackbar8);
  cv::createTrackbar("speckleWindowSize", "disparity", &speckleWindowSize, 25, on_trackbar9);
  cv::createTrackbar("disp12MaxDiff", "disparity", &disp12MaxDiff, 25, on_trackbar10);
  cv::createTrackbar("minDisparity", "disparity", &minDisparity, 25, on_trackbar11);
  */

  Mat map(700, 600, CV_8UC1);

      std::srand(time(NULL));

  for (size_t i = 0; i < 700; i++) {
    for (size_t j = 0; j < 600; j++) {
      map.at<uchar>(i, j) = rand() % 255;
    }    
  }

  imshow("image", map);

  double min;
  double max;
  cv::minMaxIdx(map, &min, &max);
  cv::Mat adjMap;
  // expand your range to 0..255. Similar to histEq();
  float scale = 255 / (max-min);
  map.convertTo(adjMap,CV_8UC1, scale, -255* min / (max - min));

  // this is great. It converts your grayscale image into a tone-mapped one, 
  // much more pleasing for the eye
  // function is found in contrib module, so include contrib.hpp 
  // and link accordingly
  cv::Mat falseColorsMap;
  applyColorMap(adjMap, falseColorsMap, cv::	COLORMAP_JET);

  cv::imshow("Out", falseColorsMap);
  waitKey(0);
}
