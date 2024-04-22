#pragma once

#include <string>

#include "space_configuration.hpp"

void evolve(SpaceConfiguration& config, float delta_time);
void evolve_by(SpaceConfiguration& config, uint32_t timesteps);

int serial_main(SpaceConfiguration config, uint32_t timesteps);
