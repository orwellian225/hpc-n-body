
cmake_minimum_required(VERSION 3.16)

project(HPC-N-Body DESCRIPTION "COMS4040A HPC Project: N-Body Simulation on CUDA and MPI" LANGUAGES CXX CUDA)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/fmt)

add_executable(nbody
    src/main.cpp src/cuda_main.cpp src/mpi_main.cpp
)

target_include_directories(nbody PUBLIC include PUBLIC lib/fmt/include)
target_link_directories(nbody PRIVATE src PRIVATE lib/fmt/src)

target_link_libraries(nbody fmt)
target_compile_options(nbody PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
set_target_properties(nbody PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)