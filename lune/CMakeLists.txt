# lune -- shared primitives consumed by maelle and verso.
#
# This is a STATIC library. To add a new primitive:
#   1. Drop the header at  include/lune/your_thing.hpp
#   2. Drop the source at  src/your_thing.cpp
#   3. Append the .cpp to the source list below.
# Headers do not need to be listed -- they're picked up via the public
# include directory.

add_library(lune STATIC
    src/mlp.cpp
)

target_include_directories(lune PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
