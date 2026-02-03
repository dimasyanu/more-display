cmake_minimum_required(VERSION 3.15)
project(MoreDisplay LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Search for EVDI library
find_library(EVDI_LIB evdi)
if (NOT EVDI_LIB)
    message(FATAL_ERROR "libevdi not found! Please install it in your machine.")
endif()

# Devine Source Files
set(SOURCES
    main.cpp
    VirtualDisplayManager.cpp
    edid_data.hpp
)

# Platform specific logic
if (APPLE)

elseif(WIN32)

else()
    # Linux
    list(APPEND SOURCES LinuxEvdiManager.hpp)
endif()

add_subdirectory(src/library)
add_subdirectory(src/executable)
