# maelle -- model architectures composed from lune primitives.
#
# This is a HEADER-ONLY (INTERFACE) library that depends on lune. To add a
# new model, just drop the header at include/maelle/your_model.hpp -- it's
# picked up via the public include dir. Models can `#include "lune/..."`
# directly: the INTERFACE link to lune below propagates lune's headers to
# consumers of maelle as well.

add_library(maelle INTERFACE)

target_include_directories(maelle INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(maelle INTERFACE lune)
