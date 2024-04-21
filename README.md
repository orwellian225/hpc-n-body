# COMS4040A HPC Project

N-Body Simulation

Project Brief: [here](./HPC_Project.pdf)

## Input Data

* Number of Bodies
* Number of executed timesteps
* Initial configuration of particles
    * Mass
    * Position
    * Velocity

## Output Data

Each timestep generates a configuration png

## Experiments

Organized into classes of executed timesteps
> At 24fps, a 10 second gif is 240 frames / timesteps

Each time class will have a different initial configuration
Each time class will be repeated at least 3 times for an average, each with a unique configuration

- $n$ - Number of bodies
- $t$ - Timesteps

|$n$|$t_1$|$t_2$|$t_3$|
|---|-----|-----|-----|
|3|240|1440|4320|
|10|240|1440|4320|
|1e3|240|1440|4320|
|1e4|240|1440|4320|
|1e5|240|1440|4320|
|1e6|240|1440|4320|

## Graphs

* Particle Configuration Representation
* Number of bodies vs Execution time
    * Organized by class of timesteps

## Generating Data

### Generate Data
```bash
python scripts/generate_configuration.py <num_bodies> <num_dimensions> <num_galaxies> <output_directory>
```

### Generate Data and Graph
```bash
scripts/gen_data.sh <num_bodies> <num_dimensions> <num_galaxies>
```