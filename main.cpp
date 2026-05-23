#include "laplacian_pyramid.h"


int main() {
    try {
        run_laplacian_pyramid();
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
    }
}
    

