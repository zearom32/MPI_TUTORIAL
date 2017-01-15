cmake_minimum_required (VERSION 2.8)
project (ParallelRank)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_library(tmpi_rank STATIC tmpi_rank.cc)
add_executable(random_rank ${PROJECT_SOURCE_DIR}/random_rank.cc)
target_link_libraries(random_rank ${MPI_C_LIBRARIES} tmpi_rank)
