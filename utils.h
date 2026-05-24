#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>


cv::Mat load_img(const std::string& path);

cv::Mat crop_img(const cv::Mat &image, const std::size_t size_crop);

int factorial(int n);

int binomial_coefficient(int n, int k);

template <typename T>
T sum(const cv::Mat& mat)
{
    T total{};
    for (int y = 0; y < mat.rows; y++) {
        const T* ptr = mat.ptr<T>(y);
        for (int x = 0; x < mat.cols; x++)
            total += ptr[x];
    }
    return total;
}

int power(const int x, const int pow);

void show(const cv::Mat &image, const std::string title="", const bool normalize=false);

void display_laplacian_pyramid(const std::vector<cv::Mat>& laplacian_pyramid, const bool normalize=false);
