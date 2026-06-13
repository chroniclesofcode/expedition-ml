# maelle -- model architectures composed from lune primitives.
#
# This is a STATIC library that depends on lune. To add a new model:
#   1. Drop the header at  include/maelle/your_model.hpp
#   2. Drop the source at  src/your_model.cpp
#   3. Append the .cpp to the source list below.
# Models can `#include "lune/..."` directly -- the PUBLIC link to lune
# below propagates lune's headers to consumers of maelle as well.

add_library(maelle STATIC
    src/transformer.cpp
)

target_include_directories(maelle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(maelle PUBLIC lune)
