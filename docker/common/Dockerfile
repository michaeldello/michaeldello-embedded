# Minimal stable Ubuntu compatible with GNU ARM Toolchain
FROM ubuntu:22.04

# Avoid user interaction during package installs
ENV DEBIAN_FRONTEND=noninteractive

# Install tools needed for build, execution, debug, version control, and 
# automation scripts
RUN apt-get update && apt-get install -y \
    make \
    cmake \
    git \
    python3 \
    python3-pip \
    openocd \
    gdb-multiarch \
    sudo \
    && apt-get clean

# Create and switch to a non-root user (but allow sudo if needed) for cleaner,
# safer development
RUN useradd -ms /bin/bash dev && \
    echo "dev ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER dev

# Set a default starting directory when launching an instance of the container
WORKDIR /workspace

# Install a PINNED native ARM64 version of gcc-arm-none-eabi from Ubuntu
# Check available versions with: apt-cache madison gcc-arm-none-eabi
RUN sudo apt-get update && \
    sudo apt-get install -y gcc-arm-none-eabi=15:10.3-2021.07-4 && \
    sudo apt-mark hold gcc-arm-none-eabi