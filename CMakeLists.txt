cmake_minimum_required(VERSION 4.2.3)
project(moredisplay VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Search for EVDI library
find_library(EVDI_LIB evdi)
if (NOT EVDI_LIB)
    message(FATAL_ERROR "libevdi not found! Please install it in your machine.")
endif()

add_subdirectory(src/library)
add_subdirectory(src/executable)
