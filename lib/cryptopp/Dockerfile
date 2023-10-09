# Použij základní obraz Ubuntu
FROM ubuntu:latest

LABEL authors="kureii"

# Nainstaluj potřebné balíčky
RUN apt-get update && apt-get install -y \
    build-essential \
    mingw-w64 \
    wget \
    file \
    unzip \
    curl

# Nastav pracovní adresář
WORKDIR /workspace

# Kopíruj Android.mk a Application.mk
COPY ../../Android.mk /workspace/Android.mk
COPY ../../Application.mk /workspace/Application.mk


# Stáhni a rozbal Android NDK
RUN curl -O https://dl.google.com/android/repository/android-ndk-r26-linux.zip && \
    unzip android-ndk-r26-linux.zip && \
    rm android-ndk-r26-linux.zip

# Nastav proměnné prostředí pro Android NDK
ENV ANDROID_NDK_HOME /workspace/android-ndk-r26
ENV PATH $PATH:$ANDROID_NDK_HOME

# Kopíruj skript pro kompilaci
COPY ./compile.sh /workspace/compile.sh
RUN chmod +x /workspace/compile.sh

