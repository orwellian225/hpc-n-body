import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import csv

def main():
    if len(sys.argv) != 3:
        print("Invalid arguments", file=sys.stderr)

    infile = sys.argv[1]
    outfile = sys.argv[2]

    regions = []
    bodies = []

    with open(infile, 'r+') as f:
        csv_reader = csv.reader(f)

        for line in csv_reader:
            if line[0] == "Region":
                regions.append(line[1:])
            elif line[0] == "Body":
                bodies.append(line[1:])

    dimension = 2

    regions = np.array(regions, dtype=float)
    bodies = np.array(bodies, dtype=float)

    masses = bodies[:, 0:1]
    positions = bodies[:, 1:(1 + dimension)]
    velocities = bodies[:, (1 + dimension):]

    normalized_masses = masses / np.max(masses)
    momentums = masses * velocities
    momentum_magnitudes = np.linalg.norm(momentums, axis = 1, keepdims = True)
    normalized_momentum_magnitudes = momentum_magnitudes / np.max(momentum_magnitudes)

    region_centres = regions[:, 3:5]
    region_widths = regions[:, 5]

    fig = plt.figure(facecolor='black', edgecolor='black')
    ax = fig.add_subplot(111)
    ax.axis('off')

    ax.scatter(positions[:, 0], positions[:, 1], s=normalized_masses, c = normalized_momentum_magnitudes, cmap='plasma', marker='o')

    for i in range(len(region_centres)):

        hx = [region_centres[i][0] - region_widths[i], region_centres[i][0] + region_widths[i]]
        hy = [region_centres[i][1], region_centres[i][1]]

        vx = [region_centres[i][0], region_centres[i][0]]
        vy = [region_centres[i][1] - region_widths[i], region_centres[i][1] + region_widths[i]]

        ax.plot(hx, hy, vx, vy, marker='', color='#0D98BA', linewidth=0.2)

    fig.savefig(outfile, bbox_inches='tight', pad_inches=0, facecolor=fig.get_facecolor(), edgecolor='none')
    plt.close(fig)

main()