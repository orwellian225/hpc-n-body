#include <string>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <fmt/core.h>

#include "fmt/base.h"
#include "space_configuration.hpp"

SpaceConfiguration SpaceConfiguration::load(uint32_t num_particles, uint8_t particle_dimension, std::string filepath) {
    Particle *particles = new Particle[num_particles];


    FILE* infile = fopen(filepath.c_str(), "r+");
    if (infile == NULL) {
        fmt::println(stderr, "File {} not opened", filepath);
        exit(EXIT_FAILURE);
    }

    char linebuffer[1024];
    size_t delimiter_pos;
    std::string line, token; 

    float mass, *positions, *velocities;
    size_t i;
    for (uint32_t particle_i = 0; particle_i < num_particles; ++particle_i) {
        positions = new float[particle_dimension];
        velocities = new float[particle_dimension];

        fgets(linebuffer, 1024, infile);
        line = std::string(linebuffer);
        line.pop_back();

        delimiter_pos = line.find(",");
        token = line.substr(0, delimiter_pos);
        line.erase(0, delimiter_pos + 1);
        mass = std::stof(token);

        for (i = 0; i < particle_dimension; ++i) {
            delimiter_pos = line.find(",");
            token = line.substr(0, delimiter_pos);
            line.erase(0, delimiter_pos + 1);
            positions[i] = std::stof(token);
        }

        for (i = 0; i < particle_dimension; ++i) {
            delimiter_pos = line.find(",");
            token = line.substr(0, delimiter_pos);
            line.erase(0, delimiter_pos + 1);
            velocities[i] = std::stof(token);
        }

        Particle new_particle = {
            mass, 
            VectorN { positions, particle_dimension }, 
            VectorN { velocities, particle_dimension },
        };

        particles[particle_i] = new_particle;
    }

    fclose(infile);

    SpaceConfiguration config = {
        particle_dimension,
        num_particles,
        particles
    };

    return config;
}

void SpaceConfiguration::save(std::string filepath) {
    FILE *outfile = fopen(filepath.c_str(), "w+");
    if (outfile == NULL) {
        fmt::println(stderr, "File {} not opened", filepath);
        exit(EXIT_FAILURE);
    }
    
    for (uint32_t i = 0; i < num_particles; ++i) {
        fmt::println(outfile, "{}", particles[i].to_csv_string());
    }

    fclose(outfile);
}

void SpaceConfiguration::free() {
    for (uint32_t i = 0; i < num_particles; ++i) {
        particles[i].free();
    }

    delete[] particles;
}

void Particle::free() {
    position.free();
    velocity.free();
}

std::string Particle::to_string() {
    std::string position_str = fmt::format("{}", position[0]);
    for (uint8_t i = 1; i < position.size; ++i)
        position_str = fmt::format("{}, {}", position_str, position[i]);

    std::string velocity_str = fmt::format("{}", velocity[0]);
    for (uint8_t i = 1; i < velocity.size; ++i)
        velocity_str = fmt::format("{}, {}", velocity_str, velocity[i]);

    return fmt::format(
        "mass = {}, p = ({}), v = ({})",
        mass, position_str, velocity_str
    );
}

std::string Particle::to_csv_string() {
    std::string position_str = fmt::format("{}", position[0]);
    for (uint8_t i = 1; i < position.size; ++i)
        position_str = fmt::format("{},{}", position_str, position[i]);

    std::string velocity_str = fmt::format("{}", velocity[0]);
    for (uint8_t i = 1; i < velocity.size; ++i)
        velocity_str = fmt::format("{},{}", velocity_str, velocity[i]);

    return fmt::format(
        "{},{},{}",
        mass, position_str, velocity_str
    );
}

VectorN* VectorN::create(uint8_t size, float initial_value) {
    VectorN *new_vector = new VectorN;
    new_vector->size = size;
    new_vector->data = new float[size];
    for (uint8_t i = 0; i < size; ++i)
        new_vector->data[i] = initial_value;

    return new_vector;
}

float& VectorN::operator[](uint8_t idx) {
    return data[idx];
}

void VectorN::free() {
    delete[] data;
}
