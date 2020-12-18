FROM cpp-build-base:0.1.0 AS build
WORKDIR /App/include
COPY include/*.h ./
WORKDIR /App/src
COPY src/*.cpp ./
WORKDIR /App
COPY CMakeLists.txt ./
RUN cmake . && make
FROM ubuntu:bionic
WORKDIR /opt/LeaderElectionProblem
COPY --from=build App/LeaderElectionProblem ./
CMD ["./LeaderElectionProblem"]