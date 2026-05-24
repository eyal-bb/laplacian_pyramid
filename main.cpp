#include "laplacian_pyramid.h"


int main() {
    try {
        run_laplacian_pyramid("image.jpg" // image_name
            , 5 // kernel_size
            , 6 // num_levels
            , 2048 // size_crop
            , "C:/Users/USER/C++_projects/laplacian_pyramid/"); // image_folder
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
    }
}
    

