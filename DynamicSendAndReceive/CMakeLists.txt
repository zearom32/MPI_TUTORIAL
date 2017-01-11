cmake_minimum_required (VERSION 2.8)
project (DynamicSendAndReceive)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(check_status ${PROJECT_SOURCE_DIR}/mpi_check_status.cc)
target_link_libraries(check_status ${MPI_C_LIBRARIES})

add_executable(probe ${PROJECT_SOURCE_DIR}/mpi_probe.cc)
target_link_libraries(probe ${MPI_C_LIBRARIES})
