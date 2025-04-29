cmake_minimum_required(VERSION 3.10)
project(dont_crash)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(dont_crash src/dont_crash.cpp)

target_link_libraries(dont_crash emc-framework)