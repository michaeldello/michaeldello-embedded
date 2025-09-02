# Docker Containers

In order to allow for Docker development containers that are optimized for either aarch64 or amd64 host machine architectures, organize this folder for potentially separate Dockerfiles.

The aarch64 Dockerfile in the common/ folder is crafted as somewhat generic, but is optimized for aarch64. That is, if it is built on an amd64 host, it would still likely work, but may not be optimized for that platform, e.g., GNU ARM toolchain version.

## GNU ARM Toolchain

### common (aarch64 or amd64)

As of this writing, ARM doesn't officially support ARM64 (aarch64) GNU Toolchain. It is unofficially available from community developed repos, or it can built from scratch. This container, being for a reference project, takes the simplest approach: Install a native version based on the underlying host architecture building the container (Docker will detect and automate this selection). To accommodate aarch64, the version can be pinned to a value that is supported by the Ubuntu ARM64 repositories.

If this version ever needs to be upgraded (pinned using a value that is supported in the Ubuntu ARM64 repositories), discover what is available and pin the new version in the common/Dockerfile, from an ARM64 host machine (e.g., Mac or rPi4):

```
# Login into the base Ubuntu container
docker run --rm -it ubuntu:22.04 bash

# Get aarch64 version candidate(s)
apt-get update
apt-cache policy gcc-arm-none-eabi
```

### amd64 (only)

TBD - The version selected for aarch64 may also be viable for amd64, or a newer version can be used/pinned and stored under a new folder, amd64/.

## Local Usage

### Building

From the repo's root dir, build and tag the image:
```
docker build -t stm32-dev -f docker/common/Dockerfile .
```

### Running

From the repo's root dir (or wherever the code of interest resides):
```
docker run --rm -it -v $(pwd):/workspace -w /workspace stm32-dev bash
```

This command:

- Runs a new instance of the container
- Destroys it on exit
- Connects to it interactively in /workspace at which a local working directory is mounted
- Starts the bash shell

## CI (GitHub Actions) Usage

### Building ARM64 - For local

- Create a Personal Access Token (PAT) in GitHub with write/read/delete::packages permissions, then use that PAT to log into the GHCR.
```
echo "<GITHUB_PAT>" | docker login ghcr.io -u <GITHUB_USERNAME> --password-stdin
```

Note that buildx is used in this scenario in order to build the container to run in Docker locally (ARM64) if desired, and in GitHub Actions (AMD64). That is, the image is built for multiple platforms (buildx is required to do this).

- Activate and confirm buildx multiplatform (skip for rebuild):
```
docker buildx create --name multiplatform --use
docker buildx ls
```

- Build, tag, and push the image and push to GHCR
```
docker buildx build \                                                                                   
  --platform linux/arm64 \ 
  -f docker/common/Dockerfile \
  -t ghcr.io/michaeldello/stm32-dev:v1.3-arm64 \
  --push \
  .
```

### Building AMD4 - For CI

Use GitHub actions Publish CI workflow.

See .github/workflows/publish-amd64.yml

## Merge ARM64 and AMD64

```
docker buildx imagetools create \
  -t ghcr.io/michaeldello/stm32-dev:v1.3 \
  -t ghcr.io/michaeldello/stm32-dev:latest \
  ghcr.io/michaeldello/stm32-dev:v1.3-amd64 \
  ghcr.io/michaeldello/stm32-dev:v1.3-arm64
```

### Running

See .github/workflows/*.yml in this repo