# lune -- shared primitives consumed by maelle and verso.
#
# This is a HEADER-ONLY (INTERFACE) library: primitives live entirely in
# include/lune/*.hpp, so there is nothing to compile and no source list to
# maintain. To add a new primitive, just drop the header at
# include/lune/your_thing.hpp -- it's picked up via the public include dir.

add_library(lune INTERFACE)

target_include_directories(lune INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
