#include <algorithm>
#include <string>

#include <stdio.h>

#include <fmt/core.h>

#include "cuda_main.hpp"
#include "mpi_main.hpp"
#include "serial_main.hpp"
#include "space_configuration.hpp"

int main(int argc, char **argv) {

    fmt::println("COMS4040A HPC Project");
    fmt::println("{:-<80}", "");
    fmt::println("Brendan Griffiths - 2426285");
    fmt::println("{:-<80}", "");

    /**
     *  Arguments
     *      0: Executable Name
     *      1: HPC Mode - CUDA / MPI
     *      2: Number of bodies
     *      3: Number of dimensions 
     *      4: Time steps
     *      5: Input filepath 
     *      6: Output filepath
     */

    if (argc != 7) {
        fmt::println(stderr, "Invalid arguments");
        fmt::println(stderr, "\tHPC Mode");
        fmt::println(stderr, "\tNumber of bodies");
        fmt::println(stderr, "\tNumber of dimensions");
        fmt::println(stderr, "\tTimesteps");
        fmt::println(stderr, "\tInput filepath");
        fmt::println(stderr, "\tOutput filepath");
        return -1;
    }

    std::string hpc_mode(argv[1]);
    uint32_t num_bodies = static_cast<uint32_t>(atoi(argv[2]));
    uint32_t num_dimensions = static_cast<uint32_t>(atoi(argv[3]));
    uint32_t num_timesteps = static_cast<uint32_t>(atoi(argv[4]));
    std::string input_filepath(argv[5]);
    std::string output_filepath(argv[6]);

    std::transform(hpc_mode.begin(), hpc_mode.end(), hpc_mode.begin(), [](unsigned char c){ return std::toupper(c); });

    fmt::println("Selected Configuration");
    fmt::println("\tHPC Mode: {}", hpc_mode);
    fmt::println("\tNumber of bodies: {}", num_bodies);
    fmt::println("\tDimension: {}", num_dimensions);
    fmt::println("\tExecuted Timesteps: {}", num_timesteps);
    fmt::println("\tInput: {}", input_filepath);
    fmt::println("\tOutput: {}", output_filepath);

    SpaceConfiguration config = SpaceConfiguration::load(num_bodies, num_dimensions, input_filepath);

    int result;
    if (hpc_mode == "SERIAL") {
        result = serial_main(config, num_timesteps, output_filepath);
    } else if (hpc_mode == "MPI") {
        result = mpi_main();
    } else if (hpc_mode == "CUDA") {
        result = cuda_main();
    } else {
        fmt::println(stderr, "Invalid HPC Mode selected. Please choose:\n\tCUDA\n\tMPI\n\tSERIAL");
        result = -1;
    }

    config.free();
    return result;
}