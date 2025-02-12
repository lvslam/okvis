/*********************************************************************************
 *  OKVIS - Open Keyframe-based Visual-Inertial SLAM
 *  Copyright (c) 2015, Autonomous Systems Lab / ETH Zurich
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of Autonomous Systems Lab / ETH Zurich nor the names of
 *     its contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Created on: Mar 31, 2015
 *      Author: Stefan Leutenegger (s.leutenegger@imperial.ac.uk)
 *********************************************************************************/

#include <iostream>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

#include <brisk/brisk.h>

#pragma GCC diagnostic pop

#include "okvis/cameras/PinholeCamera.hpp"
#include "okvis/cameras/NoDistortion.hpp"
#include "okvis/cameras/RadialTangentialDistortion.hpp"
#include "okvis/cameras/EquidistantDistortion.hpp"
#include "okvis/Frame.hpp"


TEST(Frame, functions) {

  // instantiate all possible versions of test cameras
  std::vector<std::shared_ptr<okvis::cameras::CameraBase> > cameras;
  cameras.push_back(
      okvis::cameras::PinholeCamera<okvis::cameras::NoDistortion>::createTestObject());
  cameras.push_back(
      okvis::cameras::PinholeCamera<okvis::cameras::RadialTangentialDistortion>::createTestObject());
  cameras.push_back(
      okvis::cameras::PinholeCamera<okvis::cameras::EquidistantDistortion>::createTestObject());

  for (size_t c = 0; c < cameras.size(); ++c) {

#ifdef __ARM_NEON__
    std::shared_ptr<cv::FeatureDetector> detector(
         new brisk::BriskFeatureDetector(34, 2));
#else
    std::shared_ptr<cv::FeatureDetector> detector(
        new brisk::ScaleSpaceFeatureDetector<brisk::HarrisScoreCalculator>(
            34, 2, 800, 450));
#endif

    std::shared_ptr<cv::DescriptorExtractor> extractor(
        new cv::BriskDescriptorExtractor(true, false));

    // create a stupid random image
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> eigenImage(752, 480);
    eigenImage.setRandom();
    cv::Mat image(480, 752, CV_8UC1, eigenImage.data());
    okvis::Frame frame(image, cameras.at(c), detector, extractor);

    // run
    frame.detect();
    frame.describe();
  }
}

