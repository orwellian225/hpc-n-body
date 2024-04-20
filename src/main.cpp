#include <algorithm>
#include <string>

#include <stdio.h>

#include <fmt/core.h>

#include "cuda_main.hpp"
#include "mpi_main.hpp"
#include "serial_main.hpp"

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

    if (argc != 6) {
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
    uint8_t input_mode = static_cast<uint8_t>(atoi(argv[5]));
    std::string input_filepath(argv[6]);
    std::string output_filepath(argv[7]);

    std::transform(hpc_mode.begin(), hpc_mode.end(), hpc_mode.begin(), [](unsigned char c){ return std::toupper(c); });

    fmt::println("Selected Configuration");
    fmt::println("\tHPC Mode: {}", hpc_mode);
    fmt::println("\tNumber of bodies: {}", num_bodies);
    fmt::println("\tExecuted Timesteps: {}", num_timesteps);
    fmt::println("\tInput: {}", input_filepath);
    fmt::println("\tInput: {}", output_filepath);

    if (hpc_mode == "SERIAL") {
        return serial_main();
    } else if (hpc_mode == "MPI") {
        return mpi_main();
    } else if (hpc_mode == "CUDA") {
        return cuda_main();
    } else {
        fmt::println(stderr, "Invalid HPC Mode selected. Please choose:\n\tCUDA\n\tMPI\n\tSERIAL");
    }
}