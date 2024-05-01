#pragma once

#include <vector>
#include <string>
#include <stdint.h>

struct VectorN {
    float *data;
    uint8_t size;

    float& operator[](uint8_t idx);
    

    static VectorN* create(uint8_t size, float initial_value);
    void free();
};

struct Particle {
    float mass;
    VectorN position;
    VectorN velocity;

    std::string to_string();
    std::string to_csv_string();
    void free();
};

struct SpaceConfiguration {
    uint8_t dimensions;
    uint32_t num_particles;
    Particle *particles;

    static SpaceConfiguration load(uint32_t num_particles, uint8_t particle_dimension, std::string filepath);
    void save(std::string filepath);
    void free();
};
