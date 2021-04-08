FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive
ENV LC_ALL C.UTF-8
ENV LANG C.UTF-8

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
       clang \
       gcc \
       g++ \
       g++-multilib \
       gdb \
       clang-tidy \
       clang-format \
       gcovr \
       llvm \
       sudo \
       vim \
       make \
       cmake \
       automake \
       libtool \
       git \
       less \
       curl \
       wget


# This adds the 'default' user to sudoers with full privileges:
RUN HOME=/home/default && \
    mkdir -p ${HOME} && \
    GROUP_ID=1000 && \
    USER_ID=1000 && \
    groupadd -r default -f -g "$GROUP_ID" && \
    useradd -u "$USER_ID" -r -g default -d "$HOME" -s /sbin/nologin \
    -c "Default Application User" default && \
    chown -R "$USER_ID:$GROUP_ID" ${HOME} && \
    usermod -a -G sudo default && \
    echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

ENV CC=/usr/bin/gcc
ENV CXX=/usr/bin/g++
ENV CCACHE_DIR=/build/docker_ccache
ENV LD_LIBRARY_PATH=/usr/local/lib

RUN sudo apt-get install -y flex bison libgraphviz-dev libboost-all-dev graphviz

WORKDIR /build

RUN wget https://github.com/whitemech/cudd/releases/download/v3.0.0/cudd_3.0.0_linux-amd64.tar.gz &&\
    tar -xf cudd_3.0.0_linux-amd64.tar.gz &&\
    cd cudd_3.0.0_linux-amd64 &&\
    sudo cp -P lib/* /usr/local/lib/ &&\
    sudo cp -Pr include/cudd/* /usr/local/include &&\
    rm -rf cudd_3.0.0_linux-amd64*

RUN wget https://github.com/whitemech/MONA/releases/download/v1.4-19.dev0/mona_1.4-19.dev0_linux-amd64.tar.gz &&\
    tar -xf mona_1.4-19.dev0_linux-amd64.tar.gz &&\
    cd mona_1.4-19.dev0_linux-amd64 &&\
    sudo cp -P lib/* /usr/local/lib/ &&\
    sudo cp -Pr include/* /usr/local/include &&\
    rm -rf mona_1.4-19.dev0_linux-amd64*

RUN git clone https://github.com/whitemech/Syft.git &&\
    cd Syft &&\
    git checkout syft+ &&\
    mkdir build && cd build &&\
    cmake -DCMAKE_BUILD_TYPE=Release .. &&\
    make -j &&\
    sudo make install &&\
    cd .. &&\
    rm -rf Syft

USER default

#RUN git clone --recursive https://github.com/whitemech/lydia.git /home/default/lydia
COPY --chown=default:default . /build/lydia

WORKDIR /build/lydia

RUN rm -rf build &&\
    mkdir build &&\
    cd build &&\
    cmake -DCMAKE_BUILD_TYPE=Release .. &&\
    make lydia-bin -j4 &&\
    sudo make install &&\
    cd .. &&\
    rm -rf lydia

WORKDIR /home/default

CMD ["/usr/local/bin/lydia"]
