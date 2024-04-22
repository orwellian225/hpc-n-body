#pragma once

#include <stdint.h>
#include <string>

#include "space_configuration.hpp"

enum TreeNodeType { Region, Body, Empty };

struct TreeNode {
    TreeNodeType type;
    uint8_t dimensions;
    VectorN region_centre;
    float region_width;
    uint32_t depth;

    // If Region
    TreeNode **children;
    VectorN centre_of_mass;
    float mass;

    // If Body
    Particle *node_particle;
};

// Returns the previously held body else nullptr
Particle *convert_to_region(TreeNode* tree_node);

void insert_node(TreeNode *tree_node, Particle *particle);

void print_tree(TreeNode *tree_node, uint32_t depth);
void save_tree(TreeNode *tree_node, FILE* outfile);

void free_tree(TreeNode *tree_node);
