cmake_minimum_required(VERSION 3.10)
project(dsa_project C)

# Set C standard to C11 and enable VLA
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)  # Use strict C11

# Add source files
set(SOURCES
    src/main.c
    src/rax.c
    src/utils.c
    src/constants.c
)

add_executable(debug ${SOURCES})
add_compile_options(-std=c11 -Wall -g -O0 -fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)
target_include_directories(debug PRIVATE src)

add_executable(release ${SOURCES})
add_compile_options(-std=c11 -O3)

# Set output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 