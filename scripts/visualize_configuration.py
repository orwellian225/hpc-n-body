import sys
import numpy as np
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) != 4:
        print("Invalid arguments", file=sys.stderr)

    filetype = int(sys.argv[1])
    infile = sys.argv[2]
    outdir = sys.argv[3]

    data = np.genfromtxt(infile, delimiter=',')

    properties_str = infile.split(".")[0]
    if properties_str.find("/") != 0:
        properties_str = properties_str.split("/")[-1]
    outfile = outdir + "/" + "".join([properties_str, ".pdf" if filetype == 0 else ".png"])

    properties = [int(s[1:]) for s in properties_str.split("_")]
    # num_bodies = properties[0]
    dimension = properties[1]
    # num_galaxies = properties[2]

    masses = data[:, 0:1]
    positions = data[:, 1:(1 + dimension)]
    velocities = data[:, (1 + dimension):]

    normalized_masses = masses / np.max(masses)
    momentums = masses * velocities
    momentum_magnitudes = np.linalg.norm(momentums, axis = 1, keepdims = True)
    normalized_momentum_magnitudes = momentum_magnitudes / np.max(momentum_magnitudes)

    fig = plt.figure(facecolor='black', edgecolor='black')
    ax = fig.add_subplot(111)
    ax.axis('off')

    ax.scatter(positions[:, 0], positions[:, 1], s=normalized_masses, c = normalized_momentum_magnitudes, cmap='plasma', marker='o')

    fig.savefig(outfile, bbox_inches='tight', pad_inches=0, facecolor=fig.get_facecolor(), edgecolor='none')
    plt.close(fig)


main()
