#include <iostream>
#include <opencv2/opencv.hpp>
#include "utils.h"


cv::Mat load_img(const std::string& path)
{
    cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not open: " << path << std::endl;
        return cv::Mat{};
    }
    std::cout << "Image loaded successfully!" << std::endl;
    return image;
}

cv::Mat crop_img(const cv::Mat &image, const std::size_t size_crop)
{
    auto calc_start = [](const std::size_t size_image, const std::size_t size_crop) {
        return ((size_image - size_crop) / 2);
    };
    const auto x_start = calc_start(image.cols, size_crop);
    const auto y_start = calc_start(image.rows, size_crop);
    return image(cv::Range(y_start, y_start + size_crop), cv::Range(x_start, x_start + size_crop)).clone();
}

int factorial(int n)
{
    if (n > 12)
        throw std::overflow_error("int overflow");
    if (n < 0)
        throw std::invalid_argument("n should be non-negative");
    int res{1};
    for (int i = 1; i <= n; ++i)
        res *= i;
    return res;
}

int binomial_coefficient(int n, int k)
{
    if (n < 0 || k < 0)
        throw std::invalid_argument("n and k should be positive");
    if (k > n)
        throw std::invalid_argument("k should be less than or equal to n");
    return factorial(n) / factorial(k) / factorial(n - k);
}

int power(const int x, const int pow)
{
    int res = 1;
    for (int i = 0; i < pow; ++i) 
        res *= x;
    return res;
}