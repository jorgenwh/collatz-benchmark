cmake_minimum_required(VERSION 3.16.3)
project(collatz-benchmark)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_sources
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${all_sources})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

set (CMAKE_CXX_FLAGS "-pthread -O3")
