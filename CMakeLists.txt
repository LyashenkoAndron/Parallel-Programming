cmake_minimum_required(VERSION 3.29)
project(parprog)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)

add_executable(parprog lab5/lab5.cpp)
target_link_libraries(parprog PUBLIC MPI::MPI_CXX)
