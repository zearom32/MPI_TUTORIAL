cmake_minimum_required (VERSION 2.8)
project (SendAndReceive)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(send_recv ${PROJECT_SOURCE_DIR}/mpi_send_recv.cc)
target_link_libraries(send_recv ${MPI_C_LIBRARIES})

add_executable(ring ${PROJECT_SOURCE_DIR}/mpi_ring.cc)
target_link_libraries(ring ${MPI_C_LIBRARIES})

add_executable(ping_pong ${PROJECT_SOURCE_DIR}/mpi_ping_pong.cc)
target_link_libraries(ping_pong ${MPI_C_LIBRARIES})
