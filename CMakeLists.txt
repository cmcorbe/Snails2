# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (snails2)

set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_C_STANDARD 99)

find_library(SDLLIB SDL "F:/Prog/SDL-1.2.15")

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_executable (snails
    snails.c
    pcxdecod.c
    keyconfig.c
    fps_limiter.c
    graficos.c
)

# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
target_link_libraries (snails m ${SDLLIB})
