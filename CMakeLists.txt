cmake_minimum_required(VERSION 2.8.9)
project(LeaderElectionProblem)

#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/main.cpp src/my_node.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

find_package(Threads)

add_executable(LeaderElectionProblem ${SOURCES})

target_link_libraries(LeaderElectionProblem PRIVATE Threads::Threads)