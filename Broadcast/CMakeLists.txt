cmake_minimum_required (VERSION 2.8)
project (Broadcast)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(broadcast ${PROJECT_SOURCE_DIR}/mpi_broadcast.cc)
target_link_libraries(broadcast ${MPI_C_LIBRARIES})