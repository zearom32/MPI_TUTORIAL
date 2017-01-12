cmake_minimum_required (VERSION 2.8)
project (RandomWalk)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(random_walk ${PROJECT_SOURCE_DIR}/mpi_random_walk.cc)
target_link_libraries(random_walk ${MPI_C_LIBRARIES})