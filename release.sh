#!/bin/bash

# Postav Docker obraz
docker build -t rtc .

# Spusť kontejner a proved kompilaci
# Výsledné knihovny budou uloženy do složky 'output' na hostitelském systému
docker run --rm -v $(pwd)/build:/release rtc /home/RainTextCore/compile.sh