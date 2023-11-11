#!/bin/bash

COMPILE_ALL=false
COMPILE_ANDROID=false
COMPILE_LINUX=false
COMPILE_WINDOWS=false
ANDROID_ARCHS=()
LINUX_ARCHS=()
WINDOWS_ARCHS=()
JOBS=1
PROJECT_PATH="$(realpath "$(dirname "$0")")"
declare -A ARCH_MAP=( [v7]="armeabi-v7a" [v8]="arm64-v8a" [x86]="x86" [x86_64]="x86_64" [am64]="arm64" )


usage() {
  cat << EOF
Usage: $0 [OPTION]... [PATH]
Compile RainTextCore project for various platforms and CPU architectures.

OPTIONS:
  -a, --all         Compile for all supported platforms (Android, Linux, Windows) and CPUs.
  -d, --android     Compile for Android. If no architecture is specified,
                    targets all Android architectures.
                    Supported architectures for Android:
                      all        Target all architectures (default)
                      v7         Architecture armeabi-v7a
                      v8         Architecture arm64-v8a
                      x86        Architecture x86
                      x86_64     Architecture x86_64
  -l, --linux       Compile for Linux. Defaults to x86_64 if no architecture is specified.
                    Supported architectures for Linux:
                      all        Target all architectures
                      x86        Architecture x86
                      x86_64     Architecture x86_64 (default)
                      v8         Architecture arm64-v8a
                      am64       Architecture aarch64/arm64
  -w, --windows     Compile for Windows. Defaults to x86_64 if no architecture is specified.
                    Supported architectures for Windows:
                      all        Target all architectures
                      x86        Architecture x86
                      x86_64     Architecture x86_64 (default)
                      am64       Architecture arm64
  -j [N]            Specifies the number of jobs (threads) to run simultaneously.
  [PATH]            Absolute path that specifies where the project directory is.
                    Defaults to the current directory.



Defaults:
  Android: all architectures
  Linux: x86_64
  Windows: x86_64

Examples:
  $0                            # Compile for default architectures of all platforms.
  $0 --all                      # Compile for all platforms and architectures.
  $0 -d                         # Compile for all Android architectures.
  $0 -d v8                      # Compile for Android arm64-v8a.

EOF
}

add_arch() {
  local platform="$1"
  shift
  for arch in "$@"; do
    case "$platform" in
      android) ANDROID_ARCHS+=("$arch") ;;
      linux) LINUX_ARCHS+=("$arch") ;;
      windows) WINDOWS_ARCHS+=("$arch") ;;
    esac
  done
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      usage
      exit 0
      ;;
    -a|--all)
      COMPILE_ALL=true
      shift
      ;;
    -d|--android)
      COMPILE_ANDROID=true
      shift
      if [[ "$1" =~ ^(all|v7|v8|x86|x86_64)$ ]]; then
        while [[ "$1" =~ ^(all|v7|v8|x86|x86_64)$ ]]; do
          if [ "$1" == "all" ]; then
            add_arch android "${ARCH_MAP[v7]}"
            add_arch android "${ARCH_MAP[v8]}"
            add_arch android "${ARCH_MAP[x86]}"
            add_arch android "${ARCH_MAP[x86_64]}"
          else
            add_arch android "${ARCH_MAP[$1]}"
          fi
          shift
        done
      else
        add_arch android "${ARCH_MAP[v7]}"
        add_arch android "${ARCH_MAP[v8]}"
        add_arch android "${ARCH_MAP[x86]}"
        add_arch android "${ARCH_MAP[x86_64]}"
      fi
      ;;
    -l|--linux)
      COMPILE_LINUX=true
      shift
      if [[ "$1" =~ ^(all|x86|x86_64|v8|am64)$ ]]; then
        while [[ "$1" =~ ^(all|x86|x86_64|v8|am64)$ ]]; do
          if [ "$1" == "all" ]; then
            add_arch linux "${ARCH_MAP[x86]}"
            add_arch linux "${ARCH_MAP[x86_64]}"
            add_arch linux "${ARCH_MAP[v8]}"
            add_arch linux "${ARCH_MAP[am64]}"
          else
            add_arch linux "${ARCH_MAP[$1]}"
          fi
          shift
        done
      else
        add_arch linux x86_64
      fi
      ;;
    -w|--windows)
      COMPILE_WINDOWS=true
      shift
      if [[ "$1" =~ ^(all|x86|x86_64|am64)$ ]]; then
        while [[ "$1" =~ ^(all|x86|x86_64|am64)$ ]]; do
          if [ "$1" == "all" ]; then
            add_arch windows "${ARCH_MAP[x86]}"
            add_arch windows "${ARCH_MAP[x86_64]}"
            add_arch windows "${ARCH_MAP[am64]}"
          else
            add_arch windows "${ARCH_MAP[$1]}"
          fi
          shift
        done
      else
        add_arch windows x86_64
      fi
      ;;
    -j)
      if [[ -n "$2" && "$2" =~ ^[0-9]+$ ]]; then
        JOBS="$2"
        shift 2
      else
        echo "Error: '-j' requires a numeric argument."
        usage
        exit 1
      fi
      ;;
    --) # End of all options
      shift
      break
      ;;
    -*)
      echo "Unknown option: $1"
      usage
      exit 1
      ;;
    *)
      PROJECT_PATH="$1"
      shift
      ;;
  esac
done

if [[ "$COMPILE_ALL" = true ]]; then
  COMPILE_ANDROID=true
  COMPILE_LINUX=true
  COMPILE_WINDOWS=true
  add_arch android "${ARCH_MAP[v7]}"
  add_arch android "${ARCH_MAP[v8]}"
  add_arch android "${ARCH_MAP[x86]}"
  add_arch android "${ARCH_MAP[x86_64]}"

  add_arch linux "${ARCH_MAP[x86]}"
  add_arch linux "${ARCH_MAP[x86_64]}"
  add_arch linux "${ARCH_MAP[v8]}"
  add_arch linux "${ARCH_MAP[am64]}"

  add_arch windows "${ARCH_MAP[x86]}"
  add_arch windows "${ARCH_MAP[x86_64]}"
  add_arch windows "${ARCH_MAP[am64]}"
fi

# Android
if [[ "$COMPILE_ANDROID" = true ]]; then
  cd "$PROJECT_PATH" || exit 1
  mkdir -p "$PROJECT_PATH"/release || exit 1
  mkdir -p "$PROJECT_PATH"/release/android || exit 1
  export MINSDKVERSION=26
  export NDK=/home/NDK/android-ndk-r26
  export SYSROOT=/home/NDK/android-ndk-r26/platforms/android-21/arch-arm
  export CC=/home/NDK/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
  export CXX=/home/NDK/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
  for abi in "${ANDROID_ARCHS[@]}"; do
    echo "Architecture: $abi"
    mkdir -p "$PROJECT_PATH"/release/android/"$abi" || exit 1
    cd "$PROJECT_PATH"/release/android/"$abi" || exit 1
    cmake \
      -DCMAKE_SYSTEM_NAME=Android \
      -DCMAKE_TOOLCHAIN_FILE=/home/NDK/android-ndk-r26/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI="$abi" \
      -DANDROID_ARM_NEON=ON \
      -DANDROID_PLATFORM=android-$MINSDKVERSION "$PROJECT_PATH"
    make -j "$JOBS"
  done
fi

# Linux
if [[ "$COMPILE_LINUX" = true ]]; then
  cd "$PROJECT_PATH" || exit 1
    mkdir -p "$PROJECT_PATH"/release || exit 1
    mkdir -p "$PROJECT_PATH"/release/linux || exit 1
  for arch in "${LINUX_ARCHS[@]}"; do
    echo "Architecture: $arch"
    mkdir -p "$PROJECT_PATH"/release/linux/"$arch" || exit 1
    cd "$PROJECT_PATH"/release/linux/"$arch" || exit 1
    cmake "$PROJECT_PATH" \
      -DCMAKE_SYSTEM_NAME=Linux \
      -DCMAKE_SYSTEM_PROCESSOR="$arch"
    make -j "$JOBS"
  done
fi

# Windows
if [[ "$COMPILE_WINDOWS" = true ]]; then
  echo "Compilation for Windows"
  mkdir -p "$PROJECT_PATH"/release/windows
  for arch in "${WINDOWS_ARCHS[@]}"; do
    echo "Architecture: $arch"
    mkdir -p "$PROJECT_PATH"/release/windows/"$arch"
    cd "$PROJECT_PATH"release/windows/"$arch" || exit 1
    cmake "$PROJECT_PATH" \
      -DCMAKE_SYSTEM_NAME=Windows \
      -DCMAKE_SYSTEM_PROCESSOR="$arch"
    make -j "$JOBS"
  done
fi
