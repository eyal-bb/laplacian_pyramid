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

std::pair<std::vector<cv::Mat>, cv::Mat> calc_laplacian_pyramid(const cv::Mat& image, const size_t kernel_size, const int num_levels);

float reconstruct_and_compare(const std::vector<cv::Mat>& laplacian_pyramid, const cv::Mat& kernel_1d, const cv::Mat& image);

float compare_images(const cv::Mat& image1, const cv::Mat& image2);

cv::Mat reconstruct_from_laplacian_pyramid(const std::vector<cv::Mat>& laplacian_pyramid, const cv::Mat& kernel_1d);

void run_laplacian_pyramid(const std::string image_name = "image.jpg", const int kernel_size = 5, const int num_levels = 8, const int size_crop = 2048, std::string image_folder = "", const bool show_figures = true);
