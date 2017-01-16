cmake_minimum_required (VERSION 2.8)
project (ReduceAndAllReduce)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(reduce_avg ${PROJECT_SOURCE_DIR}/reduce_avg.cc)
target_link_libraries(reduce_avg ${MPI_C_LIBRARIES})

add_executable(reduce_stddev ${PROJECT_SOURCE_DIR}/reduce_stddev.cc)
target_link_libraries(reduce_stddev ${MPI_C_LIBRARIES})