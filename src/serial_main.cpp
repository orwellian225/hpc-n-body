#include <cmath>
#include <chrono>
#include <ratio>

#include <fmt/core.h>

#include "serial_main.hpp"

#include "space_configuration.hpp"
#include "barneshut.hpp"

int serial_main(SpaceConfiguration config, uint32_t timesteps) {

    auto start = std::chrono::high_resolution_clock::now();
    evolve_by(config, timesteps);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start);

    fmt::println("\tExecution time: {} ms", duration.count());

    return 0;
}

void evolve(SpaceConfiguration& config, float delta_time) {
    TreeNode root;
    root.type = Empty;
    root.dimensions = config.dimensions;
    root.region_width = 2 * config.num_particles * 10;
    root.region_centre = VectorN::create(root.dimensions, 0.);
    root.depth = 0;
    convert_to_region(&root);

    // Insert into tree
    for (uint32_t particle_i = 0; particle_i < config.num_particles; ++particle_i) {
        insert_node(&root, &config.particles[particle_i]);
    }

    // Update Velocities
    for (uint32_t particle_i = 0; particle_i < config.num_particles; ++particle_i) {
    }

    // Update Positions
    for (uint32_t particle_i = 0; particle_i < config.num_particles; ++particle_i) {
        for (uint8_t d = 0; d < config.particles[particle_i].position.size; ++d) {
            config.particles[particle_i].position[0] += delta_time
              * config.particles[particle_i].velocity[0];
        }
    }

    free_tree(&root);
}

void evolve_by(SpaceConfiguration& config, uint32_t timesteps) {
    for (uint32_t i = 0; i < timesteps; ++i)
        evolve(config, 1./24);
}
