#!/bin/bash

# Build the image
docker build -t mylittlepony/buildforladder .

# Run container and name BlankBot
docker run --name=BlankBot mylittlepony/buildforladder

# Copy built binary from container to host, name blank_bot
docker cp BlankBot:/home/BlankBot/build/build_for_ladder/bin/BlankBot ./blank_bot

# Housekeeping
docker rm -f BlankBot
docker rmi -f mylittlepony/buildforladder

# Product of host directory structure, do not depend on.
cp ./blank_bot ../local-play-bootstrap/bots/blank_bot/
