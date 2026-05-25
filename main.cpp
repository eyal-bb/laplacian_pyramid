#include "laplacian_pyramid.h"


int main() {
    try {
        // run_laplacian_pyramid("Turtle.jpg" // image_name
        //     , "C:/Users/USER/C++_projects/"
        //     , 6 // num_levels
        //     , 5 // kernel_size
        //     , 2048 // size_crop
        //     , true); // image_folder

        auto config = load_config(std::string(PROJECT_DIR) + "/config.env");
        run_laplacian_pyramid(
            config["image_name"],
            config["image_folder"],
            std::stoi(config["num_levels"]),
            std::stoi(config["kernel_size"]),
            std::stoi(config["size_crop"]),
            config["show_figures"] == "true"
        );
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
    }
}
    

