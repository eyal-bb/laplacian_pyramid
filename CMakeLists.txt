cmake_minimum_required(VERSION 3.10)
project(laplacian_pyramid)

set(CMAKE_CXX_STANDARD 20)
set(OpenCV_DIR "C:/msys64/mingw64/lib/cmake/opencv4")

find_package(OpenCV REQUIRED)
set(Catch2_DIR "C:/Users/USER/vcpkg/installed/x64-mingw-static/share/catch2")
find_package(Catch2 3 REQUIRED)

add_compile_options(-O3)

add_library(core STATIC utils.cpp laplacian_pyramid.cpp)
target_include_directories(core PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(core PUBLIC ${OpenCV_LIBS})
target_compile_definitions(core PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}")

add_executable(laplacian_pyramid main.cpp)
target_link_libraries(laplacian_pyramid PRIVATE core)
target_compile_definitions(laplacian_pyramid PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}")

add_executable(tests tests/tests.cpp)
target_link_libraries(tests PRIVATE core Catch2::Catch2WithMain)