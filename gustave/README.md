# gustave

GPU programming exploration using [HIP-CPU](https://github.com/ROCm/HIP-CPU),
an AMD-maintained CPU emulator for the HIP runtime. HIP's API mirrors CUDA
closely enough that kernels written here translate to CUDA with mechanical
changes, so this is a way to learn the programming model without a GPU.

## Prerequisites (macOS, Apple Silicon or Intel)

HIP-CPU's upstream guidance on macOS is **Homebrew GCC + Intel TBB** — Apple
Clang's libc++ is missing pieces HIP-CPU depends on.

```bash
brew install cmake gcc tbb
```

This installs `g++-15` / `gcc-15` at `/opt/homebrew/bin/`. If a newer GCC
shows up later, swap the version suffix in the commands below.

## Build

From `gustave/`:

```bash
cmake -S . -B build -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-15
cmake --build build -j
```

The first configure pulls HIP-CPU via `FetchContent` and may take a minute.

### Known quirk: aarch64 + libco

HIP-CPU bundles `libco`, whose aarch64/arm sources unconditionally call
`VALGRIND_STACK_REGISTER` even when valgrind isn't installed. The
`CMakeLists.txt` here defines that macro to a no-op on arm so the build
just works on Apple Silicon — no action needed from you.

## Run

```bash
./build/matmul
./build/prefix_sum
```

## Editor / clangd setup

`CMAKE_EXPORT_COMPILE_COMMANDS` is on, so `build/compile_commands.json`
appears after the first configure. The `.clangd` file in this directory
points clangd at `build/` and tells it to use `g++-15` as the reference
compiler so header resolution matches the actual build.

In VS Code, install the "clangd" extension and reload the window once the
build directory exists.

## Adding a new example

1. Drop `src/your_thing.cpp` next to the existing kernels.
2. Add to `CMakeLists.txt`:
   ```cmake
   add_executable(your_thing src/your_thing.cpp)
   target_link_libraries(your_thing PRIVATE hip_cpu_rt::hip_cpu_rt)
   ```
3. `cmake --build build -j`.
