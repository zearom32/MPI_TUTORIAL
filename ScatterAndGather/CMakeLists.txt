cmake_minimum_required (VERSION 2.8)
project (ScatterAndGather)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(avg ${PROJECT_SOURCE_DIR}/mpi_avg.cc)
target_link_libraries(avg ${MPI_C_LIBRARIES})

add_executable(all_avg ${PROJECT_SOURCE_DIR}/mpi_all_avg.cc)
target_link_libraries(all_avg ${MPI_C_LIBRARIES})