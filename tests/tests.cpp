#include <catch2/catch_test_macros.hpp>
#include "../laplacian_pyramid.h"


TEST_CASE("factorial") {
    REQUIRE(factorial(0) == 1);
    REQUIRE(factorial(12) == 479001600);
    REQUIRE_THROWS_AS(factorial(-1), std::invalid_argument);
    REQUIRE_THROWS_AS(factorial(13), std::overflow_error);
}

TEST_CASE("binomial_coefficient") {
    REQUIRE(binomial_coefficient(4, 0) == 1);
}
