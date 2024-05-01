#pragma once

#include <string>

#include "space_configuration.hpp"

void evolve(SpaceConfiguration& config, float delta_time, float distance_threshold);
void evolve_by(SpaceConfiguration& config, uint32_t timesteps, float distance_threshold);

int serial_main(SpaceConfiguration config, uint32_t timesteps, std::string outfile_path);
