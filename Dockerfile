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
COPY $PWD /home/BlankBot/
RUN mkdir $WORKDIR/build_for_ladder

RUN cmake -B build/build_for_ladder -DBUILD_FOR_LADDER=ON -DSC2_VERSION=4.10.0
RUN cmake --build build/build_for_ladder -j $(nproc)

