# DockerHub Image
FROM debian:bullseye-slim
#FROM ubuntu:20.04
#FROM ubuntu:22.04
# ubuntu:24.04 ; glibc mismatch with ladder 


# Environment Variables
SHELL ["/bin/bash", "-c"]

# Installs
RUN mkdir /home/bot
WORKDIR /home/bot

RUN apt-get update -y && apt-get upgrade -y && apt-get autoremove -y
RUN apt-get install -y cmake
RUN apt-get install -y build-essential
RUN apt-get install -y git

# Download and Build Project
RUN git clone https://github.com/Nickrader/blank-bot.git
WORKDIR /home/bot/blank-bot 
RUN cmake -B build -DBUILD_FOR_LADDER=ON -DSC2_VERSION=4.10.0
RUN cmake --build build -j $(nproc)
