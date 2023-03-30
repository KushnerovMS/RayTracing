cmake_minimum_required(VERSION 3.16) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(RayTracing)

file(GLOB_RECURSE RayTracing_src
    src/Engine/*.h  src/Engine/*.cpp
    src/math/*.h    src/math/*.cpp
    )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(RayTracing src/main.cpp ${RayTracing_src})

add_subdirectory(Examples)
