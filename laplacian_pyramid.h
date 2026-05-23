#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include "utils.h"


enum class BorderMode
{
    BORDER_REFLECT_101 = 0,
    BORDER_REFLECT = 1
};

enum class ConvolutionAxis
{
    x = 0,
    y = 1
};

cv::Mat calc_kernel_1d(const int kernel_size);

int reflect_from_boundary(const int coord, const int size, BorderMode border_mode = BorderMode::BORDER_REFLECT_101);

cv::Mat blur_img_1d(const cv::Mat &image, const cv::Mat& kernel_1d, const ConvolutionAxis axis, BorderMode border_mode = BorderMode::BORDER_REFLECT_101);

cv::Mat blur_img(const cv::Mat& image, const cv::Mat& kernel_1d);

cv::Mat down_sampling(const cv::Mat &image, const cv::Mat& kernel_1d);

cv::Mat up_sampling(const cv::Mat &image, const cv::Mat& kernel_1d);

// TODO: implement a rebuilt function
std::vector<cv::Mat> calc_laplacian_pyramid(const cv::Mat& image, const cv::Mat& kernel_1d, const int num_levels);

void show(const cv::Mat &image);

float reconstruct_and_compare(const std::vector<cv::Mat>& laplacian_pyramid, const cv::Mat& kernel_1d, const cv::Mat& image);

float compare_images(const cv::Mat& image1, const cv::Mat& image2);

cv::Mat reconstruct_from_laplacian_pyramid(const std::vector<cv::Mat>& laplacian_pyramid, const cv::Mat& kernel_1d);

void run_laplacian_pyramid();
