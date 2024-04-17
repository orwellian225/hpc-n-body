import sys
import numpy as np
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) != 4:
        print("Invalid arguments", file=sys.stderr)

    infile = sys.argv[1]
    dimension = int(sys.argv[2])
    outfile = sys.argv[3]

    data = np.genfromtxt(infile, delimiter=',')

    masses = data[:, 0:1]
    positions = data[:, 1:(1 + dimension)]
    velocities = data[:, (1 + dimension):]

    normalized_masses = masses / np.max(masses)
    velocity_magnitudes = np.linalg.norm(velocities, axis = 1, keepdims = True)
    normalized_velocity_magnitudes = velocity_magnitudes / np.max(velocity_magnitudes)

    fig = plt.figure(facecolor='black', edgecolor='black')
    ax = fig.add_subplot(111)
    ax.axis('off')

    ax.scatter(positions[:, 0], positions[:, 1], s=normalized_masses, c = normalized_velocity_magnitudes, cmap='plasma', marker='o')

    fig.savefig(outfile, bbox_inches='tight', pad_inches=0, facecolor=fig.get_facecolor(), edgecolor='none')
    plt.close(fig)

main()