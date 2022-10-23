#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>

void createStereoCameraSetup(cv::Mat mtxL, cv::Mat distL, cv::Mat mtxR,
                             cv::Mat distR, cv::Mat R, cv::Mat T, cv::Mat Left_Stereo_Map1,
                             cv::Mat Left_Stereo_Map2, cv::Mat Right_Stereo_Map1, cv::Mat Right_Stereo_Map2);