#!/bin/bash

# Postav Docker obraz
docker build -t crypto-compiler .

# Spusť kontejner a proved kompilaci
# Výsledné knihovny budou uloženy do složky 'output' na hostitelském systému
docker run --rm -v $(pwd)/output:/output crypto-compiler /workspace/compile.sh
