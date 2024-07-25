#!/bin/bash

docker build -t mylittlepony/buildforladder . 
docker run --name=BlankBot mylittlepony/buildforladder
docker cp BlankBot:/home/bot/blank-bot/build/bin/BlankBot $PWD/blank_bot
docker rm -f BlankBot
docker rmi -f mylittlepony/buildforladder

# build the image
# run the image non-interactive and name container
# copy the built binary to pwd
# remove container
# remove image

