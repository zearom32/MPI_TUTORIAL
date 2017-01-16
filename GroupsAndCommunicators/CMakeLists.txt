cmake_minimum_required (VERSION 2.8)
project (GroupsAndCommunicators)

find_package(MPI REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(groups ${PROJECT_SOURCE_DIR}/groups.cc)
target_link_libraries(groups ${MPI_C_LIBRARIES})

add_executable(split ${PROJECT_SOURCE_DIR}/split.cc)
target_link_libraries(split ${MPI_C_LIBRARIES})