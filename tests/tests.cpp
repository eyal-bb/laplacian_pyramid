#include <catch2/catch_test_macros.hpp>
#include "../laplacian_pyramid.h"
#include <opencv2/opencv.hpp>


TEST_CASE("factorial") {
    REQUIRE(factorial(0) == 1);
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(12) == 479001600);
    REQUIRE_THROWS_AS(factorial(-1), std::invalid_argument);
    REQUIRE_THROWS_AS(factorial(13), std::overflow_error);
}

TEST_CASE("binomial_coefficient") {
    REQUIRE(binomial_coefficient(4, 0) == 1);
    REQUIRE(binomial_coefficient(4, 4) == 1);
    REQUIRE(binomial_coefficient(10, 4) == 210);
    REQUIRE_THROWS_AS(binomial_coefficient(4, 5), std::invalid_argument);
    REQUIRE_THROWS_AS(binomial_coefficient(4, -2), std::invalid_argument);
}

TEST_CASE("power") {
    REQUIRE(power(4, 2) == 16);
    REQUIRE(power(2, 0) == 1);
    REQUIRE_THROWS_AS(power(6, -5), std::invalid_argument);
}

TEST_CASE("calc_kernel_1d") {
    const auto kernel_1d = (cv::Mat_<float>(1, 5) << 1.0F, 4.0F, 6.0F, 4.0F, 1.0F) / 16.0F;
    REQUIRE(sum<float>(cv::abs(calc_kernel_1d(5) - kernel_1d)) < 1e-7);
    REQUIRE(std::abs(sum<float>(calc_kernel_1d(7)) - 1.0F) < 1e-7);
}

cv::Mat add_multiscale_noise(const cv::Mat& image, const int noise_levels, const float noise_level) {
    cv::Mat noise = cv::Mat::zeros(image.size(), image.type());
    for (int i = 0; i < noise_levels; ++i) {
        cv::Mat noise_low = cv::Mat::zeros(image.rows / power(2, i), image.cols / power(2, i), image.type());
        cv::randu(noise_low, 0.0f, noise_level);
        cv::resize(noise_low, noise_low, image.size());
        noise += noise_low;
    } 
    return image + noise;
}

TEST_CASE("flow") {
    constexpr std::size_t size_img{1024};
    constexpr std::size_t kernel_size{5};
    constexpr int num_levels{8};
    cv::Mat image(size_img, size_img, CV_32FC1);
    image = add_multiscale_noise(image, num_levels, 1.0F);
    const auto &[laplacian_pyramid, kernel_1d] = calc_laplacian_pyramid(image, kernel_size, num_levels);
    auto relative_error = reconstruct_and_compare(laplacian_pyramid, kernel_1d, image);

    REQUIRE(relative_error < 1e-6);
}
