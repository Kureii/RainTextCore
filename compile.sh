#!/bin/bash

cd /home/RainTextCore
mkdir -p build/windows && cd build/windows && cmake ../.. -DCMAKE_SYSTEM_NAME=Windows && make -j12

cd /home/RainTextCore
mkdir -p build/linux && cd build/linux && cmake ../.. -DCMAKE_SYSTEM_NAME=Linux && make -j12
cd /home/RainTextCore

mkdir -p build/android
export MINSDKVERSION=26
export NDK=/home/NDK/android-ndk-r26
export SYSROOT=/home/NDK/android-ndk-r26/platforms/android-21/arch-arm
export CC=/home/NDK/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
export CXX=/home/NDK/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++

export ABI=armeabi-v7a
mkdir -p build/android/armeabi-v7a && \
    cd build/android/armeabi-v7a &&\
    cmake  -DCMAKE_SYSTEM_NAME=Android  \
        -DCMAKE_TOOLCHAIN_FILE=/home/NDK/android-ndk-r26/build/cmake/android.toolchain.cmake  \
        -DANDROID_ABI=$ABI  \
        -DANDROID_ARM_NEON=ON  \
        -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
        make -j12

cd /home/RainTextCore

export ABI=arm64-v8a
mkdir -p build/android/arm64-v8a && \
    cd build/android/arm64-v8a && \
    cmake -DCMAKE_SYSTEM_NAME=Android  \
    -DCMAKE_TOOLCHAIN_FILE=/home/NDK/android-ndk-r26/build/cmake/android.toolchain.cmake  \
    -DANDROID_ABI=$ABI  \
    -DANDROID_ARM_NEON=ON  \
    -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
    make -j12

cd /home/RainTextCore

export ABI=x86
mkdir -p build/android/x86 && cd build/android/x86 && \
    cmake -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=$ABI \
        -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
    make -j12

cd /home/RainTextCore

export ABI=x86_64
mkdir -p build/android/x86_64 && \
    cd build/android/x86_64 && \
    cmake -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake  \
        -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-$MINSDKVERSION /home/RainTextCore && \
    make -j12