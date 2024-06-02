FROM ubuntu:22.04
COPY . repo/
WORKDIR repo
RUN apt update
RUN apt install -yy gcc g++ cmake
RUN cmake -H. -B_build -DTEST=ON
RUN cmake --build _build
RUN cmake --build _build --target test -- ARGS=--verbose > log.txt