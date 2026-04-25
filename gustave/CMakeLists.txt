cmake_minimum_required(VERSION 3.16)
project(gustave CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Release if the user did not pick a build type.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Emit compile_commands.json for clangd / IDE tooling.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# HIP-CPU needs Intel TBB. On macOS the upstream guidance is to use
# Homebrew GCC + Homebrew TBB; Apple Clang's libc++ is missing pieces
# HIP-CPU relies on (e.g. parallel STL backed by TBB).
if(APPLE)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR
            "HIP-CPU on macOS requires Homebrew GCC, not ${CMAKE_CXX_COMPILER_ID}.\n"
            "Configure with e.g.:\n"
            "  cmake -S . -B build \\\n"
            "    -DCMAKE_C_COMPILER=/opt/homebrew/bin/gcc-15 \\\n"
            "    -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-15")
    endif()

    # Make Homebrew TBB visible to find_package().
    execute_process(
        COMMAND brew --prefix tbb
        OUTPUT_VARIABLE BREW_TBB_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(BREW_TBB_PREFIX)
        list(PREPEND CMAKE_PREFIX_PATH "${BREW_TBB_PREFIX}")
    endif()
endif()

# Pull HIP-CPU at configure time.
include(FetchContent)
FetchContent_Declare(
    hip_cpu
    GIT_REPOSITORY https://github.com/ROCm/HIP-CPU.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(hip_cpu)

# HIP-CPU's bundled libco unconditionally calls VALGRIND_STACK_REGISTER on
# aarch64 / arm even when valgrind.h is absent (the x86 / sjlj variants
# guard it correctly; the ARM ones don't). Stub it out as a no-op when
# valgrind isn't installed -- it's purely a tool-hint macro.
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
    add_compile_options("SHELL:-D VALGRIND_STACK_REGISTER(a,b)=((void)0)")
endif()

# --- Examples -----------------------------------------------------------

add_executable(matmul src/matmul.cpp)
target_link_libraries(matmul PRIVATE hip_cpu_rt::hip_cpu_rt)

add_executable(prefix_sum src/prefix_sum.cpp)
target_link_libraries(prefix_sum PRIVATE hip_cpu_rt::hip_cpu_rt)
