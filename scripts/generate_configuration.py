import sys
import csv
import numpy as np

def generate_galaxies(num_bodies, dimension, num_galaxies):
    data = []

    # U-Quadratic distribution fucntion https://en.wikipedia.org/wiki/U-quadratic_distribution accessed on 17 April 2024
    a = -(num_bodies)**2
    b = num_bodies**2
    pdf = lambda x: (12 / (b - a)**3) * (x - (b + a) / 2)**2

    galaxy_centres = []
    while len(galaxy_centres) < num_galaxies:
        centre = []
        for _ in range(dimension):
            centre.append(np.random.uniform(-(num_bodies) * 10, num_bodies * 10))

        if all(np.linalg.norm(np.array(centre) - np.array(prev_centre)) >= num_bodies * 10 / num_galaxies for prev_centre in galaxy_centres):
            galaxy_centres.append(centre)


    print(galaxy_centres)
    for _ in range(num_bodies):
        particle = [
            abs(np.random.normal(50, 100)), # Mass
        ]

        galaxy_i = 0 if num_galaxies == 1 else np.random.randint(0, num_galaxies)
        for d in range(dimension): # position
            random_position = np.random.normal( galaxy_centres[galaxy_i][d], num_bodies)
            particle.append(random_position)

        for _ in range(dimension): # velocity
            particle.append(np.random.uniform(0, 100) / particle[0] ) # generate a momentum and then convert to velocity

        data.append(particle)

    return data

def generate_uniform(num_bodies, dimension):
    data = []
    for _ in range(num_bodies):
        particle = [
            abs(np.random.normal(50, 100)), # Mass
        ]

        for _ in range(dimension): # position
            particle.append(np.random.uniform(-(num_bodies)**2 , (num_bodies)**2 ))

        for _ in range(dimension): # velocity
            particle.append(np.random.uniform(0, 100) / particle[0] ) # generate a momentum and then convert to velocity

        data.append(particle)

    return data

def main():
    if len(sys.argv) != 5:
        print(f'Incorrect arguments provided', file=sys.stderr)
        return

    style = int(sys.argv[1])
    num_bodies = int(sys.argv[2])
    dimension = int(sys.argv[3])
    output_file = str(sys.argv[4])

    with open(output_file, "w+") as of:
        csv_writer = csv.writer(of)

        if style > 0:
            data = generate_galaxies(num_bodies, dimension, style)
        else:
            data = generate_uniform(num_bodies, dimension)

        csv_writer.writerows(data)

main()