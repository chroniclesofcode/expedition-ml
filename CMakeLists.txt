cmake_minimum_required(VERSION 3.16)
project(expedition_ml CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The ML trio builds normally; the gustave target builds and runs CUDA examples.
add_subdirectory(lune)
add_subdirectory(maelle)
add_subdirectory(verso)

add_custom_target(gustave
    COMMAND ${CMAKE_COMMAND} -S "${CMAKE_SOURCE_DIR}/gustave" -B "${CMAKE_BINARY_DIR}/gustave"
    COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/gustave" --target run
    USES_TERMINAL
    VERBATIM
)
