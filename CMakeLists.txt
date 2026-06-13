cmake_minimum_required(VERSION 3.16)
project(expedition_ml CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The ML trio. gustave/ has its own self-contained build and is not
# included here on purpose -- it pulls in HIP-CPU and we don't want
# that dependency for the from-scratch ML work.
add_subdirectory(lune)
add_subdirectory(maelle)
add_subdirectory(verso)
