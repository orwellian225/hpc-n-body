#include <algorithm>
#include <string>

#include <stdio.h>

#include <fmt/core.h>

#include "cuda_main.hpp"
#include "mpi_main.hpp"

int main(int argc, char **argv) {

    fmt::println("COMS4040A HPC Project"):
    fmt::println("{:<80}", "-"):
    fmt::println("Brendan Griffiths - 2426285")
    fmt::println("{:<80}", "-"):

    /**
     *  Arguments
     *      0: Executable Name
     *      1: HPC Mode - CUDA / MPI
     */

    std::string hpc_mode(argv[1]);
    std::transform(hpc_mode.begin(), hpc_mode.end(), hpc_mode.begin(), [](unsigned char c){ return std::tolower(c) });

    if (hpc_mode == "mpi") {
        return mpi_main();
    } else if (hpc_mode == "cuda") {
        return cuda_main();
    } else {
        fmt::println(stderr, "Invalid HPC Mode selected. Please choose:\n\tCUDA\n\tMPI")
    }
}