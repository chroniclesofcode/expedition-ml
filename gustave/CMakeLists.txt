cmake_minimum_required(VERSION 3.16)
project(gustave LANGUAGES NONE)

if(NOT APPLE OR NOT CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64)$")
    message(FATAL_ERROR "Gustave requires Apple Silicon and macOS 14 or newer.")
endif()

find_program(CUMETALC NAMES cumetalc)
if(NOT CUMETALC)
    message(FATAL_ERROR
        "Install cuMetal: brew install lulzx/tap/cumetal, then run cumetal doctor. "
        "Or configure with -DCUMETALC=/path/to/cumetalc.")
endif()

# cuMetal 0.8 needs an explicit include path for its device compilation.
get_filename_component(cumetalc_real "${CUMETALC}" REALPATH)
get_filename_component(cumetal_bin "${cumetalc_real}" DIRECTORY)
find_path(CUMETAL_INCLUDE_DIR cuda_runtime.h
    HINTS "${cumetal_bin}/../include" "$ENV{CUMETAL_ROOT}/include")
if(NOT CUMETAL_INCLUDE_DIR)
    message(FATAL_ERROR "Set -DCUMETAL_INCLUDE_DIR=/path/to/cumetal/include.")
endif()

# Call cuMetal directly: no NVIDIA toolkit or CMake CUDA detection needed.
enable_testing()
file(GLOB sources CONFIGURE_DEPENDS "src/*.cu")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "src/*.h" "src/*.hpp" "src/*.cuh")
add_custom_target(run
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose --output-on-failure
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    USES_TERMINAL
    VERBATIM
)
foreach(source IN LISTS sources)
    get_filename_component(example "${source}" NAME_WE)
    set(binary "${CMAKE_CURRENT_BINARY_DIR}/bin/${example}")
    add_custom_command(
        OUTPUT "${binary}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/bin"
        COMMAND "${CUMETALC}" -I "${CUMETAL_INCLUDE_DIR}" "${source}" -o "${binary}"
        DEPENDS "${source}" ${headers} "${CUMETALC}"
        VERBATIM
    )
    add_custom_target(${example} ALL DEPENDS "${binary}")
    add_dependencies(run ${example})
    add_test(NAME ${example} COMMAND "${binary}")
endforeach()
