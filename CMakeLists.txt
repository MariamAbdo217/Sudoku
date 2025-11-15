cmake_minimum_required(VERSION 4.0)
project(Sudoko_Dancing_Links)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Code_library subdirectory first
add_subdirectory(Code_library)

# Main executable (compression/decompression program)
add_executable(Sudoko_Dancing_Links main.cpp)
target_link_libraries(Sudoko_Dancing_Links PRIVATE Code_library)