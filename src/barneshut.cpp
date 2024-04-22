#include <fmt/core.h>
#include <cmath>

#include "barneshut.hpp"

void insert_node(TreeNode *tree_node, Particle *particle) {
    // If its a region, then add the particle to the relevent child node
    if (tree_node->type == Region) {
        uint8_t child_idx = 0;

        // Fetch child idx
        // 2D: child_idx = (x >= xp) * 1 + (y >= yp) * 2
        // 3D: child_idx = (x >= xp) * 1 + (y >= yp) * 2 + (z >= zp) * 4
        for (uint8_t i = 0; i < tree_node->dimensions; ++i)
            child_idx += (tree_node->region_centre[i] <= particle->position[i]) * pow(2, i);

        for (uint8_t i = 0; i < tree_node->dimensions; ++i) {
            tree_node->centre_of_mass[i] *= tree_node->mass;
            tree_node->centre_of_mass[i] += particle->position[i] * particle->mass;
        }

        tree_node->mass += particle->mass;
        for (uint8_t i = 0; i < tree_node->dimensions; ++i)
            tree_node->centre_of_mass[i] /= tree_node->mass;

        insert_node(tree_node->children[child_idx], particle);

    // If its a body, split it into a region, and then add both particles 
    } else if (tree_node->type == Body) {
        Particle *previous_particle = convert_to_region(tree_node);
        insert_node(tree_node, previous_particle);
        insert_node(tree_node, particle);
    } else {
        tree_node->type = Body;
        tree_node->node_particle = particle;
        tree_node->children = nullptr;
    }
}

void print_tree(TreeNode *tree_node, uint32_t depth) {
    if (tree_node == nullptr)
        return;

    std::string node_str = fmt::format("{} | {}:", 
        depth, 
        tree_node->type == Region ? "Region" : tree_node->type == Body ? "Body" : "Empty"
    );

    if (tree_node->type == Region) {
        std::string position_str = fmt::format("{}", tree_node->region_centre[0]);
        for (uint8_t i = 1; i < tree_node->dimensions; ++i) {
            position_str = fmt::format("{}, {}", position_str, tree_node->region_centre[i]);
        }

        std::string com_str = fmt::format("{}", tree_node->centre_of_mass[0]);
        for (uint8_t i = 1; i < tree_node->dimensions; ++i) {
            com_str = fmt::format("{}, {}", com_str, tree_node->centre_of_mass[i]);
        }

        node_str = fmt::format("{} mass = {}, CoM = ({}), p = ({})", node_str, tree_node->mass, com_str, position_str);

        fmt::println("{}", node_str);
        for (uint32_t i = 0; i < static_cast<uint32_t>(std::pow(2., tree_node->dimensions)); ++i)
            if (tree_node->children[i] != nullptr)
                print_tree(tree_node->children[i], depth + 1);

    } else if (tree_node->type == Body) {
        node_str = fmt::format("{} {}", node_str, tree_node->node_particle->to_string());
        fmt::println("{}", node_str);
    }

}

void save_tree(TreeNode *tree_node, FILE* outfile) {
    std::string node_str = fmt::format("{}", 
        tree_node->type == Region ? "Region" : tree_node->type == Body ? "Body" : "Empty"
    );

    if (tree_node->type == Region) {
        std::string position_str = fmt::format("{}", tree_node->region_centre[0]);
        for (uint8_t i = 1; i < tree_node->dimensions; ++i) {
            position_str = fmt::format("{},{}", position_str, tree_node->region_centre[i]);
        }

        std::string com_str = fmt::format("{}", tree_node->centre_of_mass[0]);
        for (uint8_t i = 1; i < tree_node->dimensions; ++i) {
            com_str = fmt::format("{},{}", com_str, tree_node->centre_of_mass[i]);
        }

        node_str = fmt::format("{},{},{},{},{}", node_str, tree_node->mass, com_str, position_str, tree_node->region_width);

        fmt::println(outfile, "{}", node_str);
        for (uint32_t i = 0; i < static_cast<uint32_t>(std::pow(2., tree_node->dimensions)); ++i)
            if (tree_node->children[i] != nullptr)
                save_tree(tree_node->children[i], outfile);

    } else if (tree_node->type == Body) {
        node_str = fmt::format("{},{}", node_str, tree_node->node_particle->to_csv_string());
        fmt::println(outfile, "{}", node_str);
    }
}

Particle *convert_to_region(TreeNode* tree_node) {
    Particle *previous_particle = tree_node->node_particle;
    tree_node->node_particle = nullptr;

    tree_node->type = Region;
    tree_node->children = new TreeNode*[static_cast<uint32_t>(std::pow(2., tree_node->dimensions))];

    for (uint32_t i = 0; i < static_cast<uint32_t>(std::pow(2., tree_node->dimensions)); ++i) {
        tree_node->children[i] = new TreeNode;
        tree_node->children[i]->region_centre = VectorN::create(tree_node->dimensions, 0.);
        tree_node->children[i]->type = Empty;
        tree_node->children[i]->node_particle = nullptr;
        tree_node->children[i]->depth = 1;
        tree_node->children[i]->dimensions = tree_node->dimensions;
        tree_node->children[i]->region_width = tree_node->region_width / 2.;

        // Left of old dim centre point (-1), right of old dim centre point (0)
        // Works for any dimensions
        // 2D example
        // 0 0 = 0 | 1 0 = 1    -1 -1 |  0 -1
        // ----------------- => -------------
        // 1 0 = 2 | 1 1 = 2    -1  0 |  0  0
        // 
        // i      | j | (i >> j) & 1
        // 0 = 00 | 0 | 0
        // 0 = 00 | 1 | 0
        // 1 = 01 | 0 | 1
        // 1 = 01 | 1 | 0
        // 2 = 10 | 0 | 0
        // 2 = 10 | 1 | 1
        // 3 = 11 | 0 | 1
        // 3 = 11 | 1 | 1
        for (uint8_t j = 0; j < tree_node->dimensions; ++j)
            tree_node->children[i]->region_centre[j] = tree_node->region_centre[j] + (((i >> j) & 1) == 1 ? 1 : -1) * tree_node->children[i]->region_width;
    }

    tree_node->centre_of_mass = VectorN::create(tree_node->dimensions, 0.);
    tree_node->mass = 0.;

    return previous_particle;
}

void free_tree(TreeNode *tree_node) {
    if (tree_node == nullptr) 
        return;

    tree_node->type = Empty;
    tree_node->region_centre.free();
    tree_node->centre_of_mass.free();

    if (tree_node->type == Region) {
        for (uint32_t i = 0; i < static_cast<uint32_t>(std::pow(2., tree_node->dimensions)); ++i)
            free_tree(tree_node->children[i]);

        delete[] tree_node->children;
    }

    return;
}
