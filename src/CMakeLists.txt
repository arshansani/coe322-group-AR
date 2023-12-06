# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.12)

# Project's name
project(RouteSimulation)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Add the executable
add_executable(RouteSimulation route_simulation.cpp route.cpp)
