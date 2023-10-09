#!/bin/bash

# Stáhni zdrojové kódy Crypto++
wget https://github.com/weidai11/cryptopp/archive/refs/tags/CRYPTOPP_8_9_0.zip
unzip CRYPTOPP_8_9_0.zip
cd cryptopp-CRYPTOPP_8_9_0

mkdir -p /output/linux /output/windows /output/android/armeabi-v7a /output/android/arm64-v8a /output/android/x86 /output/android/x86_64

# Kompilace pro Android
cd /workspace # nebo kamkoliv jsi zkopíroval Android.mk a Application.mk
$ANDROID_NDK_HOME/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk

# Kopíruj zkompilované soubory do výstupního adresáře
mv ./libs/armeabi-v7a/libcryptopp.so /output/android/armeabi-v7a/
mv ./libs/arm64-v8a/libcryptopp.so /output/android/arm64-v8a/
mv ./libs/x86/libcryptopp.so /output/android/x86/
mv ./libs/x86_64/libcryptopp.so /output/android/x86_64/

# Kompilace pro Linux
cd cryptopp-CRYPTOPP_8_9_0
make -j12 -f GNUmakefile
mv ./libcryptopp.a /output/linux/

# Kompilace pro Windows
ls GNUmakefile
make -j12 -f ./GNUmakefile-cross CXX=x86_64-w64-mingw32-g++ AR=x86_64-w64-mingw32-ar RANLIB=x86_64-w64-mingw32-ranlib
mv ./libcryptopp.a /output/windows/

