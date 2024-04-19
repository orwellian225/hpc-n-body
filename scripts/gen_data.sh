#!/bin/sh

python3 scripts/generate_configuration.py $1 $2 $3 "data" | xargs -I {} sh -c "echo {}; python3 scripts/visualize_configuration.py 0 {} \"graphs\""
