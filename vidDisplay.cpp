//
//  vidDisplay.cpp
//  Created by Hongchao Fang 10/08/23.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core/core.hpp"

#include <string>
#include <tuple>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>

#include "dirent.h"
#include "filter.h"

#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

bool sortbyfirst(const std::tuple<int, String>& a, const std::tuple<int, String>& b) {
    return (std::get<0>(a) < std::get<0>(b));
}

std::vector<std::string> sortHelper(std::vector<std::tuple <float, std::string>> input, int numImages) {
    std::vector<std::string> result;
    sort(input.begin(), input.end(), sortbyfirst);
    for (int i = 1; i < input.size() && i <= numImages; i++) {
        std::cout << std::get<1>(input.at(i)) << " : " << std::to_string(std::get<0>(input.at(i)))  << std::endl;
        result.push_back(std::get<1>(input.at(i)));
    }
    return result;
}

std::vector<std::string> readAllFiles(char* path) {
    std::vector<std::string> result;
    char dirname[256];
    char buffer[256];
    FILE* fp;
    DIR* dirp;
    struct dirent* dp;
    int i;

    strcpy_s(dirname, path);
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    while ((dp = readdir(dirp)) != NULL) {
        if (strstr(dp->d_name, ".jpg") || strstr(dp->d_name, ".png") || strstr(dp->d_name, ".ppm") || strstr(dp->d_name, ".tif")) {
            strcpy_s(buffer, dirname);
            strcat_s(buffer, "/");
            strcat_s(buffer, dp->d_name);
            result.push_back(buffer);
        }
    }
    return(result);
}
int main(int argc, char* argv[])
{
    if (argc != 6) {
        return -2;
    }
    int bin = atoi(argv[3]);
    std::string path = argv[2];
    std::string target = argv[1];
    target = path + "/" + target;
    std::vector<std::string> database_filt_dir = readAllFiles(argv[2]);
    std::cout << target;
    cv::Mat image = imread(target, IMREAD_COLOR);

    std::cout << "The target image is "<< target << std::endl;
    std::cout << "The target dataset is "<< path << std::endl;
    std::string matchingMethod = argv[4];
    int numImages = atoi(argv[5]);
    std::vector<std::tuple <float, std::string>> result;
    

    if(matchingMethod == "baseline") {
        std::cout << "The first "<< argv[5]<< " similar img using baseline matching on" << target << "are: " << std::endl;
        result = baseline_matching(image, database_filt_dir);
    }

    if (matchingMethod == "single_histogram") {
        std::cout << "The first "<< argv[5]<< " similar img using single channel histogram matching on" << target << "are: " << std::endl;
        result = histogram_matching(image, database_filt_dir, bin);
    }

    if (matchingMethod == "multi_histogram") {
        std::cout << "The first "<< argv[5]<< " similar img using multi channel histogram matching on" << target << "are: " << std::endl;
        result = multi_histogram_matching(image, database_filt_dir, bin);
    }

    if (matchingMethod == "texture_matching") {
        std::cout << "The first "<< argv[5]<< " similar img using multi texture histogram matching on" << target << "are: " << std::endl;
        result = texture_color_matching(image, database_filt_dir, bin);
    }

    if (matchingMethod == "custom_matching_1") {
        std::cout << "The first "<< argv[5]<< " similar img using custom_match_1 on" << target << "are: " << std::endl;
        result = customized_matching(image, database_filt_dir, bin, 2);
    }

    if (matchingMethod == "custom_matching_2") {
        std::cout << "The first "<< argv[5]<< " similar img using custom_match_2 on" << target << "are: " << std::endl;
        result = customized_matching(image, database_filt_dir, bin, 1);
    }

    std::vector<std::string> sorted_result = sortHelper(result, numImages);
}


