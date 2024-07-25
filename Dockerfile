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

# Copy working directory host, to workdir container
#COPY . .
### So it seems that it is probably better if I: bind/mount/volume???
### Need to learn up on that.  Cause copy is usually expensive operation
### Meh??? https://docs.docker.com/reference/dockerfile/#volume
### Seems like this copies too, so maybe a moot point.
### These guys support COPY for deployement situation liek mine
### https://www.reddit.com/r/docker/comments/hml037/volumes_vs_addcopy_for_local_development/
### Bbbuuttt... Right now I am using it quasi as dev environment (rebuilding over and over)
### So if was the Reddit sez is tru?  Then Volume isn't 'copying', just mounting or referencing.

RUN mkdir build/build_for_ladder

RUN cmake -B build/build_for_ladder -DBUILD_FOR_LADDER=ON -DSC2_VERSION=4.10.0
RUN cmake --build build/build_for_ladder -j $(nproc)
