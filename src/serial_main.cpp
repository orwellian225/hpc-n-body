#include <cmath>

#include <fmt/core.h>

#include "serial_main.hpp"

#include "space_configuration.hpp"
#include "barneshut.hpp"

int serial_main(SpaceConfiguration config, uint32_t timesteps) {

    TreeNode root;
        root.type = Empty;
        root.dimensions = config.dimensions;
        root.region_width = 2 * config.num_particles * 10;
        root.region_centre = VectorN::create(root.dimensions, 0.);
        root.depth = 0;
        convert_to_region(&root);

    for (uint32_t particle_i = 0; particle_i < config.num_particles; ++particle_i) {
        insert_node(&root, &config.particles[particle_i]);
    }

    print_tree(&root, 0);
    FILE* tree_file = fopen("../data/demo_tree.csv", "w+");
    save_tree(&root, tree_file);
    fclose(tree_file);

    return 0;
}

void evolve(SpaceConfiguration& config) {

}

void evolve_by(SpaceConfiguration& config, uint32_t timesteps) {
    for (uint32_t i = 0; i < timesteps; ++i)
        evolve(config);
}