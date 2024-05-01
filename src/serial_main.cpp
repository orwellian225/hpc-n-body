#include <cmath>
#include <chrono>
#include <ratio>
#include <string>

#include <fmt/core.h>

#include "serial_main.hpp"

#include "space_configuration.hpp"
#include "barneshut.hpp"

int serial_main(SpaceConfiguration config, uint32_t timesteps, std::string outfile_path) {

    auto start = std::chrono::high_resolution_clock::now();
    evolve_by(config, timesteps, 0.5);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(end - start);

    fmt::println("\tExecution time: {} ms", duration.count());

    config.save(outfile_path);

    return 0;
}

void evolve(SpaceConfiguration& config, float delta_time, float distance_threshold) {
    TreeNode root;
    root.type = Empty;
    root.dimensions = config.dimensions;
    root.region_width = 2 * config.num_particles * 10;
    root.region_centre = VectorN::create(root.dimensions, 0.);
    root.depth = 0;
    convert_to_region(&root);

    // Insert into tree and reset the force of the particle
    for (uint32_t particle_i = 0; particle_i < config.num_particles; ++particle_i) {
        insert_node(&root, &config.particles[particle_i]);

    }

    // Update Velocities
    VectorN *particle_force = VectorN::create(config.dimensions, 0.);
    for (uint32_t particle_i = 0; particle_i < config.num_particles; ++particle_i) {
        calculate_force(&root, &config.particles[particle_i], *particle_force, distance_threshold);

        for (uint8_t d = 0; d < config.dimensions; ++d)
            config.particles[particle_i].velocity[d] += (*particle_force)[d];
    }
    particle_force->free();
    delete particle_force;

    // Update Positions
    for (uint32_t particle_i = 0; particle_i < config.num_particles; ++particle_i) {
        for (uint8_t d = 0; d < config.dimensions; ++d) {
            config.particles[particle_i].position[d] += delta_time
              * config.particles[particle_i].velocity[d];
        }
    }

    free_tree(&root);
}

void evolve_by(SpaceConfiguration& config, uint32_t timesteps, float distance_threshold) {
    for (uint32_t i = 0; i < timesteps; ++i)
        evolve(config, 1., distance_threshold);
}
