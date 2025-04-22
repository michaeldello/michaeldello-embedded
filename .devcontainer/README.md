# .devcontainer folder

This folder contains VSCode configuration file(s) needed for local host development in a Docker container(s) specified in the docker/ folder.

The build platform is specified as "aarch64" for my existing development environment. The Dockerfile is crafted as somewhat generic, but optimized for aarch64. That is, if it is built on an amd64 host, it would still work, but may not be optimized for that platform.