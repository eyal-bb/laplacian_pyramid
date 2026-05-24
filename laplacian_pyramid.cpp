#include <iostream>
#include <opencv2/opencv.hpp>
#include "laplacian_pyramid.h"


cv::Mat calc_kernel_1d(const int kernel_size)
{
    if (kernel_size < 2 || kernel_size > 12)
        throw std::invalid_argument("Kernel should be odd and in range [2, 12]");

    cv::Mat kernel_1d = cv::Mat::zeros(1, kernel_size, CV_32FC1);
    float* ptr = kernel_1d.ptr<float>(0);
    for (int k=0; k < kernel_size; k++) {
        ptr[k] = static_cast<float>(binomial_coefficient(kernel_size - 1, k));
    }
    kernel_1d /= sum<float>(kernel_1d);
    return kernel_1d;
}

int reflect_from_boundary(const int coord, const int size, BorderMode border_mode)
{
    int coord_reflected = coord;
    
    if (coord < 0) 
        coord_reflected = - coord - static_cast<int>(border_mode);// static_cast<int>(border_mode): 0 for BORDER_REFLECT_101, 1 for BORDER_REFLECT
    else if (coord >= size)
        coord_reflected = 2 * size - 2 - coord + static_cast<int>(border_mode);
    return coord_reflected;
}

cv::Mat blur_img_1d(const cv::Mat &image, const cv::Mat& kernel_1d, const ConvolutionAxis axis, BorderMode border_mode)
{
    const int del = kernel_1d.total() / 2;
    const int rows = image.rows;
    const int cols = image.cols;
    cv::Mat image_blurred{image.size(), image.type(), cv::Scalar(0.0F)};
    const float* kernel_ptr = kernel_1d.ptr<float>(0);
    if (axis == ConvolutionAxis::x) {
        for (int y = 0; y < rows; ++y) {
            const float* img_ptr = image.ptr<float>(y);
            float* conv_ptr = image_blurred.ptr<float>(y);
            for (int shift = -del; shift <= del; shift++) {
                const auto kernel_val = kernel_ptr[shift + del];
                for (int x = 0; x < cols; ++x)
                {
                    auto x_shifted = reflect_from_boundary(x + shift, cols, border_mode);
                    conv_ptr[x] += img_ptr[x_shifted] * kernel_val;
                }
            }
        }
    }
    else if (axis == ConvolutionAxis::y) {
        for (int y = 0; y < rows; ++y) {
            float* conv_ptr = image_blurred.ptr<float>(y);
            for (int shift = -del; shift <= del; shift++) {
                const auto kernel_val = kernel_ptr[shift + del];
                const auto y_shifted = reflect_from_boundary(y + shift, rows, border_mode);
                const float* img_ptr = image.ptr<float>(y_shifted);
                for (int x = 0; x < cols; ++x)
                    conv_ptr[x] += img_ptr[x] * kernel_val;
            }
        }
    }
    else
    {        
        throw std::invalid_argument("Invalid convolution axis");
    }
    
    return image_blurred;
}

cv::Mat blur_img(const cv::Mat& image, const cv::Mat& kernel_1d)
{
    auto image_blurred = blur_img_1d(image, kernel_1d, ConvolutionAxis::x);
    image_blurred = blur_img_1d(image_blurred, kernel_1d, ConvolutionAxis::y);
    return image_blurred;
}

cv::Mat down_sampling(const cv::Mat &image, const cv::Mat& kernel_1d)
{   
    cv::Mat image_blurred = blur_img(image, kernel_1d);
    cv::Mat image_down_sampled{image.rows / 2, image.cols / 2, image.type(), cv::Scalar(0.0F)};
    const int rows = image_down_sampled.rows;
    const int cols = image_down_sampled.cols;
    for (int y=0; y < rows; ++y) {
        const float* image_blurred_ptr = image_blurred.ptr<float>(y * 2);
        float* res_ptr = image_down_sampled.ptr<float>(y);
        for (int x=0; x < cols; ++x) {
            res_ptr[x] = image_blurred_ptr[2 * x];
        }
    }
    return image_down_sampled;
}

cv::Mat up_sampling(const cv::Mat &image, const cv::Mat& kernel_1d)
{
    cv::Mat image_up_sampled{image.rows * 2, image.cols * 2, image.type(), cv::Scalar(0.0F)};
    const int rows = image.rows;
    const int cols = image.cols;
    for (int y=0; y < rows; ++y) {
        const float* img_ptr = image.ptr<float>(y);
        float* res_ptr = image_up_sampled.ptr<float>(2 * y);
        for (int x=0; x < cols; ++x) {
            res_ptr[2 * x] = img_ptr[x] * 4.0F;
        }
    }
    
    image_up_sampled = blur_img(image_up_sampled, kernel_1d);
    return image_up_sampled;
}

std::pair<std::vector<cv::Mat>, cv::Mat> calc_laplacian_pyramid(const cv::Mat& image, const size_t kernel_size, const int num_levels)
{
    if (kernel_size % 2 == 0)
        throw std::invalid_argument("Kernel should be odd");

    if (image.type() != CV_32FC1)
        throw std::invalid_argument("Image should be of type float");

    if (num_levels < 1 || num_levels > 8)
        throw std::invalid_argument("Number of levels should be in range [1, 8]");

    if ((image.cols % power(2, num_levels) != 0) || (image.rows % power(2, num_levels) != 0))
        throw std::invalid_argument("Image size should be of divisible by 2 num_levels (" + std::to_string(num_levels) + ") times");

    const auto kernel_1d = calc_kernel_1d(kernel_size);

    // check kernel_1d validity, although it should be valid if calc_kernel_1d is implemented correctly
    if (kernel_1d.rows != 1 || kernel_1d.cols % 2 == 0)
        throw std::invalid_argument("Kernel should be a row vector of odd size and type float");
        
    std::vector<cv::Mat> laplacian_pyramid{};
    auto g_low = image;
    for (int level = 0; level < num_levels; ++level)
    {
        const auto g_high = down_sampling(g_low, kernel_1d);
        const auto g_high_up_sampled = up_sampling(g_high, kernel_1d);
        laplacian_pyramid.push_back(g_low - g_high_up_sampled);
        g_low = g_high;
    }
    laplacian_pyramid.push_back(g_low);
    return {laplacian_pyramid, kernel_1d};
}

cv::Mat upsample_n_times(const cv::Mat& image, const cv::Mat& kernel_1d, const int n)
{
    cv::Mat res = image;
    for (int i = 0; i < n; ++i)
    {
        res = up_sampling(res, kernel_1d);
    }
    return res;
}

float reconstruct_and_compare(const std::vector<cv::Mat>& laplacian_pyramid, const cv::Mat& kernel_1d, const cv::Mat& image)
{
    auto sum_laplacian_pyramid = reconstruct_from_laplacian_pyramid(laplacian_pyramid, kernel_1d);
    auto relative_error = compare_images(sum_laplacian_pyramid, image);
    return relative_error;
}

float compare_images(const cv::Mat& image1, const cv::Mat& image2)
{
    if ((image1.size() != image2.size()) || (image1.type() != image2.type()))
        throw std::invalid_argument("Images should have the same size and type");

    return std::abs(sum<float>(cv::abs(image1 - image2)) / sum<float>(cv::abs(image1)));
}

cv::Mat reconstruct_from_laplacian_pyramid(const std::vector<cv::Mat>& laplacian_pyramid, const cv::Mat& kernel_1d)
{
    cv::Mat sum_laplacian_pyramid = cv::Mat::zeros(laplacian_pyramid[0].size(), laplacian_pyramid[0].type());
    int levels_up = 0;
    for (const auto& laplacian_pyramid_level : laplacian_pyramid) {
        sum_laplacian_pyramid += upsample_n_times(laplacian_pyramid_level, kernel_1d, levels_up);
        levels_up++;
    }
    return sum_laplacian_pyramid;
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
        show(laplacian_pyramid_level, "level" + std::to_string(level), normalize);
        ++level;
    }
}

void run_laplacian_pyramid(const std::string image_name, const int kernel_size, const int num_levels, const int size_crop, std::string image_folder, const bool show_figures)
{
    if (image_folder == "")
        image_folder = std::string(PROJECT_DIR) + "/";
    cv::Mat image = load_img(image_folder + image_name);
    image = crop_img(image, size_crop);
    image.convertTo(image, CV_32FC1, 1 / 255.0F);
    
    auto start = std::chrono::high_resolution_clock::now();
    const auto &[laplacian_pyramid, kernel_1d] = calc_laplacian_pyramid(image, kernel_size, num_levels);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time: " << duration.count() << " ms" << std::endl;

    auto relative_error = reconstruct_and_compare(laplacian_pyramid, kernel_1d, image);
    std::cout << "relative error in reconstruction- " << relative_error << std::endl;

    if (show_figures)
    {
        show(image, "image", true);
        display_laplacian_pyramid(laplacian_pyramid, true);
    }
}
