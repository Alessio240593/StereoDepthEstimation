/****************************************************************************
 * Copyright (C) 2022 by Alessio Zattoni                                    *
 *                                                                          *
 * This file is part of CrossCorrelation.                                   *
 *                                                                          *
 *   CrossCorrelation is free software: you can redistribute it and/or      *
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
 * @file CrossCorrelation.hpp
 * @author Alessio Zattoni
 * @date 
 * @brief Questo file contiene delle funzioni per eseguire la cross-correlazione
 *
 * ...
 */


#ifndef STEREODEPTH_STEREO_Mat
#define STEREODEPTH_STEREO_Mat

#include <opencv2/opencv.hpp>


class StereoMat {
public: 

    StereoMat(cv::Mat left, cv::Mat right) 
        : _left(left)
        , _right(right)
    {

    }


private: 
    cv::Mat _left;
    cv::Mat _right;
};

#endif // STEREODEPTH_STEREO_Mat