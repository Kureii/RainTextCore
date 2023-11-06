FROM ubuntu:22.04

LABEL authors="kureii"
LABEL maintainer="ad.tomik@seznam.cz"
LABEL version="0.1.0"

RUN apt-get update && apt-get install -y \
    curl \
    wget \
    build-essential \
    libgtest-dev \
    mingw-w64 \
    ninja-build  \
    python3  \
    python3-distutils  \
    openjdk-8-jdk  \
    qemu-kvm \
    unzip \
    lib32z1 \
    libpthread-stubs0-dev \
    ca-certificates \
    gpg

RUN wget https://apt.kitware.com/kitware-archive.sh
RUN chmod +x ./kitware-archive.sh
RUN bash ./kitware-archive.sh

RUN apt-get install -y cmake

# Android NDK
ENV JDK_PATH=/usr/lib/jvm/java-8-openjdk-amd64/bin
ENV PATH=$PATH:$JDK_PATH
WORKDIR /home/NDK
RUN curl -O https://dl.google.com/android/repository/android-ndk-r26-linux.zip
RUN unzip android-ndk-r26-linux.zip
RUN rm android-ndk-r26-linux.zip
ENV ANDROID_NDK_HOME=/workspace/android-ndk-r26
ENV PATH=$PATH:$ANDROID_NDK_HOME


COPY . /home/RainTextCore

WORKDIR /home/RainTextCore

RUN chmod +x /home/RainTextCore/compile.sh


RUN mkdir -p build
