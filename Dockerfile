# DockerHub Image
FROM debian:bullseye-slim
#FROM ubuntu:20.04
#FROM ubuntu:22.04
# ubuntu:24.04 ; glibc mismatch with ladder 


# Environment Variables
SHELL ["/bin/bash", "-c"]

# Update OS and Tools
RUN apt-get update -y && apt-get upgrade -y && apt-get autoremove -y
RUN apt-get install -y cmake
RUN apt-get install -y build-essential
RUN apt-get install -y git

RUN mkdir /home/BlankBot
WORKDIR /home/BlankBot
ADD * /home/BlankBot

RUN cmake -B build -DBUILD_FOR_LADDER=ON -DSC2_VERSION=4.10.0
#RUN cmake --build build -j $(nproc)
