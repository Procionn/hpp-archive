cmake_minimum_required(VERSION 3.16)
project(hpp-archive)

add_subdirectory(src)

target_include_directories(HPP_ARCHIVE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
