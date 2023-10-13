//
//  filter.cpp
//  Created by Hongchao Fang 10/08/23.
//


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core/core.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <filter.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
using namespace cv;


float histogram_diff_vec(std::vector<int> a, std::vector<int> b) {
    float result = 0;
    for (int i = 0; i < a.size(); i++) {
        result += abs(a[i] - b[i]);
    }
    return result;
}


std::vector<int> multi_histogram_matching_helper(cv::Mat img, int bin) {
    std::vector<int> vector (bin * bin * bin, 0);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            cv::Vec3b p = img.at<cv::Vec3b>(i, j);
            int a = bin * std::ceil((float)p[0]) / (1 + p[1] + p[2] + p[0]);
            int b = bin * std::ceil((float)p[1]) / (1 + p[1] + p[2] + p[0]);
            int c = bin * std::ceil((float)p[2]) / (1 + p[1] + p[2] + p[0]);
            int index = a * bin * bin + b * bin + c;
            vector[index] += 1;
        }
    }
    return vector;
}



std::vector<int> multi_histogram_matching_helper_C141(cv::Mat img, int bin) {
    std::vector<int> vector(bin * bin * bin, 0);
    for (int i = -70; i <= 70; i++) {
        for (int j = -70; j <= 70; j++) {
            cv::Vec3b p = img.at<cv::Vec3b>(img.rows/2 + i, img.cols/2 + j);
            int a = bin * std::ceil((float)p[0]) / (1 + p[1] + p[2] + p[0]);
            int b = bin * std::ceil((float)p[1]) / (1 + p[1] + p[2] + p[0]);
            int c = bin * std::ceil((float)p[2]) / (1 + p[1] + p[2] + p[0]);
            int index = a * bin * bin + b * bin + c;
            vector[index] += 1;
        }
    }
    return vector;
}

//multi-histogram_matching
std::vector<std::tuple <float, std::string>> multi_histogram_matching(cv::Mat target, std::vector<std::string> dir, int bin) {
    std::vector<int> target_hist = multi_histogram_matching_helper(target, bin);
    std::vector<int> target_hist_c15 = multi_histogram_matching_helper_C141(target, bin);

    std::vector<std::tuple <float, std::string>> result;
    for (std::string path : dir) {
        std::tuple <float, std::string> temp;
        std::vector<int> goal = multi_histogram_matching_helper(cv::imread(path, cv::IMREAD_COLOR), bin);
        std::vector<int> goal_C15 = multi_histogram_matching_helper_C141(cv::imread(path, cv::IMREAD_COLOR), bin);
        float diff = histogram_diff_vec(target_hist, goal) / (target.cols * target.rows);
        diff += histogram_diff_vec(target_hist_c15, goal_C15) / (141 * 141);

        temp = make_tuple(diff, path);
        result.push_back(temp);
    }
    return result;
}


float histogram_diff_Mat(cv::Mat a, cv::Mat b) {
    float result = 0;
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j <= a.cols; j++) {
            int a_v = a.at<int>(i, j);
            int b_v = b.at<int>(i, j);
            result += abs(a_v - b_v);
        }
    }
    return result;
}


cv::Mat single_histogram_matching_helper(cv::Mat img, int bin) {
    int d[2] = { bin, bin };
    cv::Mat result = Mat::zeros(2, d, CV_64F);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            cv::Vec3b p = img.at<cv::Vec3b>(i, j);
            int a = bin * std::ceil((float)p[0]) / (1 + p[1] + p[2] + p[0]);
            int b = bin * std::ceil((float)p[1]) / (1 + p[1] + p[2] + p[0]);
            result.at<int>(a, b) += 1;
        }
    }
    return result;
}

//histogram matching
std::vector<std::tuple <float, std::string>> histogram_matching(cv::Mat target, std::vector<std::string> dir, int bin) {
    cv::Mat target_hist = single_histogram_matching_helper(target, bin);
    std::vector<std::tuple <float, std::string>> result;
    for (std::string path : dir) {
        std::tuple <float, std::string> temp;
        cv::Mat goal = single_histogram_matching_helper(cv::imread(path, cv::IMREAD_COLOR), bin);
        float diff = histogram_diff_Mat(target_hist, goal);
        temp = make_tuple(diff, path);
        result.push_back(temp);
    }
    return result;
}


std::vector<int> baseline_matching_helper(cv::Mat img) {
    std::vector<int> result;
    for (int i = -4; i <= 4; i++) {
        for (int j = -4; j <= 4; j++) {
            for (int k = 0; k < 3; k++) {
                result.push_back(img.at<cv::Vec3b>(img.rows / 2 + i, img.cols / 2 + j)[k]);
            }
        }
    }
    return result;
}


std::vector<std::tuple <float, std::string>> baseline_matching(cv::Mat target, std::vector<std::string> dir) {
    std::vector<std::tuple <float, std::string>> result;
    std::vector<int> t = baseline_matching_helper(target);
    for (std::string path : dir) {
        std::tuple <float, std::string> temp;
        std::vector<int> v = baseline_matching_helper(cv::imread(path, cv::IMREAD_COLOR));
        float diff = 0;
        for (int i = 0; i < v.size(); i++) {
            diff += pow(t[i] - v[i], 2);
        }
        temp = make_tuple(diff, path);
        result.push_back(temp);
    }
    return result;
}

int greyscale(cv::Mat& src, cv::Mat& dst) {
    dst.create(src.size(), src.type());

    for (int i = 0; i < src.rows; i++) {
        cv::Vec3b* row_src = src.ptr<cv::Vec3b>(i);
        cv::Vec3b* row_dst = dst.ptr<cv::Vec3b>(i);

        for (int j = 0; j < src.cols; j++) {
            int blue = row_src[j][0];
            row_dst[j][0] = blue;
            row_dst[j][1] = blue;
            row_dst[j][2] = blue;
        }
    }

    return 0;
}

cv::Mat sobelY3x3(cv::Mat& src) {
    cv::Mat dst;
    dst.create(src.size(), src.type());
    float kernely[3][3] = 
    { { 1, 2, 1 },
    { 0,  0,  0 },
    { -1,  -2,  -1 } };
    int n = src.rows;
    int m = src.cols;
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            double pix[3] = { 0, 0, 0 };
            
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    cv::Vec3b p = src.at<cv::Vec3b>(i - 1 + a, j - 1 + b);
                    for (int k = 0; k < 3; k++) {
                        pix[k] += p[k] * kernely[a][b] / 8;
                    }
                }
            }
            for (int k = 0; k < 3; k++) {
                
                dst.at<cv::Vec3b>(i, j)[k] = abs(pix[k]);
            }
            
        }
    }
    return dst;
}

cv::Mat sobelX3x3(cv::Mat& src) {
    cv::Mat dst;
    dst.create(src.size(), src.type());
    float kernelx[3][3] = 
    { { -1, 0, 1 },
    { -2, 0, 2 },
    { -1, 0, 1 } };
    int n = src.rows;
    int m = src.cols;
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            double pix[3] = { 0, 0, 0 };

            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    cv::Vec3b p = src.at<cv::Vec3b>(i - 1 + a, j - 1 + b);
                    for (int k = 0; k < 3; k++) {
                        pix[k] += p[k] * kernelx[a][b] / 8;
                    }
                }
            }
            for (int k = 0; k < 3; k++) {
                
                dst.at<cv::Vec3b>(i, j)[k] = abs(pix[k]);
            }

        }
    }
    return dst;
}


cv::Mat magnitude(cv::Mat& input) {
    cv::Mat sy = sobelY3x3(input);
    cv::Mat sx = sobelX3x3(input);
    cv::Mat dst;
    dst.create(sx.size(), sx.type());
    int n = sx.rows;
    int m = sx.cols;
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < m - 1; j++) {
            double pix[3] = { 0, 0, 0 };
            cv::Vec3b p_1 = sx.at<cv::Vec3b>(i, j);
            cv::Vec3b p_2 = sy.at<cv::Vec3b>(i, j);
            for (int k = 0; k < 3; k++) {
                dst.at<cv::Vec3b>(i, j)[k] = sqrt(p_1[k] * p_1[k] + p_2[k] * p_2[k]);
            }

        }
    }
    return dst;

}

// Texture and Color matching
std::vector<std::tuple <float, std::string>> texture_color_matching(cv::Mat target, std::vector<std::string> dir, int bin) {
    
    Mat temp_pic;
    Mat temp_pic_1;
    convertScaleAbs(magnitude(target), temp_pic);
    cvtColor(temp_pic, temp_pic_1, COLOR_BGR2GRAY);
    std::vector<int> target_hist = multi_histogram_matching_helper(target, bin);
    std::vector<int> target_hist_gray = multi_histogram_matching_helper(temp_pic_1, bin);
    std::vector<std::tuple <float, std::string>> result;
    for (std::string path : dir) {
        std::tuple <float, std::string> temp;
        cv::Mat cur = cv::imread(path, cv::IMREAD_COLOR);
        convertScaleAbs(magnitude(cur), temp_pic);
        cvtColor(temp_pic, temp_pic_1, COLOR_BGR2GRAY);
        std::vector<int> v_hist = multi_histogram_matching_helper(cur, bin);
        std::vector<int> v_hist_gray = multi_histogram_matching_helper(temp_pic_1, bin);
        float diff = histogram_diff_vec(target_hist, v_hist) * 0.5;
        diff += histogram_diff_vec(target_hist_gray, v_hist_gray) * 0.5;
        temp = make_tuple(diff, path);
        result.push_back(temp);
    }
    return result;
}


std::vector<int> multi_histogram_matching_helper_C15(cv::Mat img, int bin) {
    std::vector<int> vector(bin * bin * bin, 0);
    for (int i = -100; i <= 100; i++) {
        for (int j = -100; j <= 100; j++) {
            cv::Vec3b p = img.at<cv::Vec3b>(img.rows / 2 + i, img.cols / 2 + j);
            int a = bin * std::ceil((float)p[0]) / (1 + p[1] + p[2] + p[0]);
            int b = bin * std::ceil((float)p[1]) / (1 + p[1] + p[2] + p[0]);
            int c = bin * std::ceil((float)p[2]) / (1 + p[1] + p[2] + p[0]);
            int index = a * bin * bin + b * bin + c;
            vector[index] += 1;
        }
    }
    return vector;
}


int countSingleColor(int color, cv::Mat img) {
    int result = 0;
    for (int i = -100; i <= 100; i++) {
        for (int j = -100; j <= 100; j++) {
            cv::Vec3b p = img.at<cv::Vec3b>(img.rows / 2 + i, img.cols / 2 + j);
            if ((float)p[color] / (1 + p[1] + p[2] + p[0]) * 100 > 50) {
                result += 1;
            }
            
        }
    }
    return result;
}

// custom design
std::vector<std::tuple <float, std::string>> customized_matching(cv::Mat target, std::vector<std::string> dir, int bin, int color) {

    Mat temp_pic;
    Mat temp_pic_1;
    target = (target);
    convertScaleAbs(magnitude(target), temp_pic);
    cvtColor(temp_pic, temp_pic_1, COLOR_BGR2GRAY);
    std::vector<int> target_hist = multi_histogram_matching_helper(target, bin);
    std::vector<int> target_hist_C15 = multi_histogram_matching_helper_C15(target,bin);
    std::vector<int> target_hist_gray = multi_histogram_matching_helper(temp_pic_1, bin);
    int count = countSingleColor(color, target);
    std::vector<std::tuple <float, std::string>> result;
    for (std::string path : dir) {
        std::tuple <float, std::string> temp;
        cv::Mat cur = (cv::imread(path, cv::IMREAD_COLOR));
        convertScaleAbs(magnitude(cur), temp_pic);
        cvtColor(temp_pic, temp_pic_1, COLOR_BGR2GRAY);
        std::vector<int> v_hist = multi_histogram_matching_helper(cur, bin);
        std::vector<int> v_hist_C15 = multi_histogram_matching_helper_C15(cur, bin);

        std::vector<int> v_hist_gray = multi_histogram_matching_helper(temp_pic_1, bin);
        if (abs(count - countSingleColor(color, cur)) > 2000) {
            float diff = histogram_diff_vec(target_hist, v_hist) * 0.5;
                diff += histogram_diff_vec(target_hist_C15, v_hist_C15) * 0.5;
                diff += histogram_diff_vec(target_hist_gray, v_hist_gray) * 1;
                temp = make_tuple(-diff, path);
                result.push_back(temp);
        }
    }
    return result;
}