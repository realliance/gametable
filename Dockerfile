FROM alpine:3.12.3 as pistache

WORKDIR /build

RUN apk add alpine-sdk cmake git openssl-dev

RUN git clone https://github.com/oktal/pistache.git && \
    cd pistache && \
    git checkout 9a65f40975fafca5bb5370ba6d0d00f42cbc4356 && \
    mkdir build && \
    mkdir prefix && \
    cd build && \
    cmake -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DPISTACHE_BUILD_EXAMPLES=false \
        -DPISTACHE_BUILD_TESTS=false \
        -DPISTACHE_BUILD_DOCS=false \
        -DPISTACHE_USE_SSL=true \
        -DCMAKE_INSTALL_PREFIX=$PWD/../prefix \
        ../ && \
    make -j && \
    make install

FROM alpine:3.12.3 as builder

# Install from edge for g++ 10
RUN apk add g++ gcc --repository=http://dl-cdn.alpinelinux.org/alpine/edge/main
RUN apk add alpine-sdk cmake git openssl-dev spdlog-dev rapidjson-dev

WORKDIR /deps

COPY --from=pistache /build/pistache/prefix pistache

WORKDIR /build

ADD . .

RUN CMAKE_PREFIX_PATH=/deps/pistache:$CMAKE_PREFIX_PATH cmake -S . -B build -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ && \
    cmake --build build

FROM alpine:3.12.3

EXPOSE 8080

RUN apk add spdlog-dev rapidjson-dev openssl-dev

WORKDIR /app

COPY --from=pistache /build/pistache/prefix/include /usr/include
COPY --from=pistache /build/pistache/prefix/lib64 /usr/lib
COPY --from=builder /build/build/_deps/libmahjonghttp-build/libmahjonghttp.so /usr/lib/
COPY --from=builder /build/build/_deps/libmahjong-build/libmahjong.so /usr/lib/

COPY --from=builder /build/build/gametable .

ENTRYPOINT /app/gametable
