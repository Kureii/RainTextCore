FROM ubuntu:22.04

LABEL authors="kureii"
LABEL maintainer="ad.tomik@seznam.cz"
LABEL version="0.1.0"

RUN apt-get update && apt-get install -y \
    curl \
    wget \
    build-essential \
    cmake \
    libgtest-dev \
    mingw-w64 \
    ninja-build  \
    python3  \
    python3-distutils  \
    openjdk-8-jdk  \
    qemu-kvm \
    unzip \
    lib32z1 \
    libpthread-stubs0-dev

ENV JDK_PATH=/usr/lib/jvm/java-8-openjdk-amd64/bin
ENV PATH=$PATH:$JDK_PATH

#RUN wget https://developer.download.nvidia.com/compute/cuda/12.2.2/local_installers/cuda_12.2.2_535.104.05_linux.run
#RUN sh cuda_12.2.2_535.104.05_linux.run


WORKDIR /home/NDK

# Stáhni a rozbal Android NDK
RUN curl -O https://dl.google.com/android/repository/android-ndk-r26-linux.zip
RUN unzip android-ndk-r26-linux.zip
RUN rm android-ndk-r26-linux.zip

# Nastav proměnné prostředí pro Android NDK
ENV ANDROID_NDK_HOME=/workspace/android-ndk-r26
ENV PATH=$PATH:$ANDROID_NDK_HOME


COPY . /home/RainTextCore

WORKDIR /home/RainTextCore


RUN mkdir -p build

# windows compilation
RUN mkdir -p build/windows && cd build/windows && cmake ../.. -DCMAKE_SYSTEM_NAME=Windows && make

# linux compilation
RUN mkdir -p build/linux && \
    cd build/linux && \
    cmake ../.. -DCMAKE_SYSTEM_NAME=Linux && \
    make && \
    cd /home/RainTextCore

# andorid compilation
RUN mkdir -p build/android
ENV MINSDKVERSION=24
ENV NDK=/home/NDK/android-ndk-r26
ENV SYSROOT=/home/NDK/android-ndk-r26/platforms/android-21/arch-arm
ENV CC=/home/NDK/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
ENV CXX=/home/NDK/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++

ENV ABI=armeabi-v7a
RUN mkdir -p build/android/armeabi-v7a && \
    cd build/android/armeabi-v7a &&\
    cmake  -DCMAKE_SYSTEM_NAME=Android  \
        -DCMAKE_TOOLCHAIN_FILE=/home/NDK/android-ndk-r26/build/cmake/android.toolchain.cmake  \
        -DANDROID_ABI=$ABI  \
        -DANDROID_ARM_NEON=ON  \
        -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
        make

RUN cd /home/RainTextCore

ENV ABI=arm64-v8a
RUN mkdir -p build/android/arm64-v8a && \
    cd build/android/arm64-v8a && \
    cmake -DCMAKE_SYSTEM_NAME=Android  \
    -DCMAKE_TOOLCHAIN_FILE=/home/NDK/android-ndk-r26/build/cmake/android.toolchain.cmake  \
    -DANDROID_ABI=$ABI  \
    -DANDROID_ARM_NEON=ON  \
    -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
    make

RUN cd /home/RainTextCore

ENV ABI=x86
RUN mkdir -p build/android/x86 && cd build/android/x86 && \
    cmake -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=$ABI \
        -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
    make

RUN cd /home/RainTextCore

ENV ABI=x86_64
RUN mkdir -p build/android/x86_64 && \
    cd build/android/x86_64 && \
    cmake -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake  \
        -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
    make
