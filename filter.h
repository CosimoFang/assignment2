//
//  filter.h
//  Created by Hongchao Fang 10/08/23.
//
#ifndef __filter_h
#define __filter_h

#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
std::vector<std::tuple <float, std::string>> baseline_matching(cv::Mat target, std::vector<std::string> dir);
std::vector<std::tuple <float, std::string>> histogram_matching(cv::Mat target, std::vector<std::string> dir, int bin);
std::vector<std::tuple <float, std::string>> multi_histogram_matching(cv::Mat target, std::vector<std::string> dir, int bin);
std::vector<std::tuple <float, std::string>> texture_color_matching(cv::Mat target, std::vector<std::string> dir, int bin);
std::vector<std::tuple <float, std::string>> customized_matching(cv::Mat target, std::vector<std::string> dir, int bin, int color);


#endif#pragma once
