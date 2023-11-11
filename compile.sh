#!/bin/bash

COMPILE_ALL=false
COMPILE_LINUX=false
COMPILE_WINDOWS=false
LINUX_ARCHS=()
WINDOWS_ARCHS=()
JOBS=1
PROJECT_PATH="$(realpath "$(dirname "$0")")"
declare -A ARCH_MAP=( [v8]="arm64-v8a" [x86]="x86" [x86_64]="x86_64" [am64]="arm64" )


usage() {
  cat << EOF
Usage: $0 [OPTION]... [PATH]
Compile RainTextCore project for various platforms and CPU architectures.

OPTIONS:
  -a, --all         Compile for all supported platforms (Linux, Windows) and CPUs.
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
  Linux: x86_64
  Windows: x86_64

Examples:
  $0                            # Compile for default architectures of all platforms.
  $0 --all                      # Compile for all platforms and architectures.
  $0 -l                         # Compile for all Linux architectures.
  $0 -l v8                      # Compile for Linux arm64-v8a.

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
  COMPILE_LINUX=true
  COMPILE_WINDOWS=true

  add_arch linux "${ARCH_MAP[x86]}"
  add_arch linux "${ARCH_MAP[x86_64]}"
  add_arch linux "${ARCH_MAP[v8]}"
  add_arch linux "${ARCH_MAP[am64]}"

  add_arch windows "${ARCH_MAP[x86]}"
  add_arch windows "${ARCH_MAP[x86_64]}"
  add_arch windows "${ARCH_MAP[am64]}"
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
