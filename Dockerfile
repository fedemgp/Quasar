# Extracted from https://cloud.google.com/run/docs/quickstarts/build-and-deploy#c++_1

# We chose Alpine to build the image because it has good support for creating
# statically-linked, small programs.
FROM ubuntu:20.04 as base 

# Create separate targets for each phase, this allows us to cache intermediate
# stages when using Google Cloud Build, and makes the final deployment stage
# small as it contains only what is needed.
FROM base AS devtools

# Install the typical development tools and some additions:
#   - ninja-build is a backend for CMake that often compiles faster than
#     CMake with GNU Make.
#   - Install the boost libraries.
RUN apt-get -y update \
    &&  DEBIAN_FRONTEND=noninteractive apt-get --no-install-recommends -y install \
        apt-utils \
        bc                          \
        build-essential             \
        curl                        \
        debhelper                   \
        locales                     \
        cmake \
        git \
        gcc \
        g++ \
        libc-dev \
        tar \
        libcpprest-dev \
        libcpprest2.10 \
        libgtest-dev \
        googletest-tools \
        googletest \
    &&  echo "en_US.UTF-8 UTF-8" > /etc/locale.gen  \
    &&  locale-gen en_US.UTF-8                      \
    &&  /usr/sbin/update-locale LANG=en_US.UTF-8    \
    &&  DEBIAN_FRONTEND=noninteractive dpkg-reconfigure locales

# Copy the source code to /v/source and compile it.
FROM devtools AS builder
COPY . /v/source
WORKDIR /v/source

# Run the CMake configuration step, setting the options to create
# a statically linked C++ program
RUN cmake -S/v/source -B/v/binary \
    -DCMAKE_BUILD_TYPE=Release

# Compile the binary and strip it to reduce its size.
RUN cmake --build /v/binary
RUN strip /v/binary/quasar
RUN ls /v/binary

ENTRYPOINT [ "/v/binary/quasar" ]