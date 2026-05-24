#include <iostream>
#include <opencv2/opencv.hpp>
#include "utils.h"


cv::Mat load_img(const std::string& path)
{
    cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);

    if (image.empty())
        throw std::invalid_argument("Image is empty");

    std::cout << "Image loaded successfully!" << std::endl;
    return image;
}

 // TODO: crop_img is implemented using cv::Range since it is not used for the core algorithm (not implemented manually using raw pointers)
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
    if (pow < 0)
        throw std::invalid_argument("pow should be non-negative");
    int res = 1;
    for (int i = 0; i < pow; ++i) 
        res *= x;
    return res;
}

void show(const cv::Mat &image, const std::string title, const bool normalize)
{
    cv::Mat display = image;
    if (normalize)
    {
        cv::normalize(image, display, 0.0f, 1.0f, cv::NORM_MINMAX);
    }
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, 512, 512);
    // cv::moveWindow(title, 0, 0);
    cv::imshow(title, image);
    std::cout << "Press any key to continue" << std::endl;
    const int key = cv::waitKey(0);
}

void display_laplacian_pyramid(const std::vector<cv::Mat>& laplacian_pyramid, const bool normalize) {
    int level{};
    for (const auto& laplacian_pyramid_level : laplacian_pyramid) {
        show(laplacian_pyramid_level, "level " + std::to_string(level), normalize);
        ++level;
    }
}

cv::Mat prepare_image(const cv::Mat &image, const int size_crop)
{
    cv::Mat image_cropped{};
    if (size_crop > 0)
        image_cropped = crop_img(image, size_crop);
        std::cout << "Croping image to " << std::to_string(size_crop) << " size" << std::endl;
    image_cropped.convertTo(image_cropped, CV_32FC1, 1 / 255.0F);
    return image_cropped;
}
