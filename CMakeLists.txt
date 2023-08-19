# Minimum required CMake version
cmake_minimum_required(VERSION 3.5)

# Project name
project(HASHTABLE)

# Add a library
add_library(hashLibrary SHARED
    hash.c
)

# Add include directories for the library
target_include_directories(hashLibrary PUBLIC
    hash.h
)

# Add executable
add_executable(hashDemo
    test.c
)

#Add compiling options
target_compile_options(hashDemo PRIVATE
    -g
)

# Link the library to the executable
target_link_libraries(hashDemo PRIVATE
    hashLibrary
)