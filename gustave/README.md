# gustave

Small CUDA learning examples running on the Apple GPU through
[cuMetal](https://github.com/Lulzx/cuda-metal). Ordinary `.cu` files use
`cudaMalloc`, `cudaMemcpy`, and `kernel<<<grid, block>>>()` launches.
The examples are deliberately unoptimised; cuMetal is experimental and
supports a subset of CUDA.

## Install (once)

Requires Apple Silicon, macOS 14+, Homebrew, and Xcode with its Metal tools.
See the [upstream toolchain guide](https://github.com/Lulzx/cuda-metal/blob/main/docs/build.md).

```bash
xcrun --find metal
xcrun --find metallib
brew install cmake lulzx/tap/cumetal
cumetal doctor
```

If the Metal tools are missing, install/select full Xcode. On recent Xcode,
`xcodebuild -downloadComponent metalToolchain` installs the Metal component.
Resolve any failures from `cumetal doctor` before building.

## Compile and run

From the repository root, configure once:

```bash
cmake -S . -B build
```

Then use this command every time you want to **build and run all examples**:

```bash
cmake --build build --target gustave
```

It discovers every `gustave/src/*.cu` file, compiles changed examples,
and runs them all, showing their output. Build or runtime failures make
this command fail. You do not need to edit CMake when adding an example.
Executables live in `build/gustave/bin/`.

For just one example after building:

```bash
./build/gustave/bin/vecadd
```

Each executable checks every result and exits nonzero on a CUDA error or
mismatch. Expected results: vector addition ends at `159`, the matrix
product contains `128` throughout, and the inclusive scan ends at `136`.

To see cuMetal's GPU dispatch diagnostics:

```bash
CUMETAL_TRACE_GPU=1 cmake --build build --target gustave
```

Look for `device=apple_gpu` and `launch_success=true` as well as correct
results. This is GPU execution through Metal. All three examples were
verified with cuMetal 0.8.0 on an Apple M4, macOS 15.7.4, and Xcode 16.2.

If you prefer working only inside `gustave/`, the equivalent loop is:

```bash
cmake -S . -B build-cumetal          # once, from gustave/
cmake --build build-cumetal --target run
```

CMake invokes `cumetalc` directly, supplying its CUDA header directory;
cuMetal handles Metal compilation and runtime linking. A custom installation can be selected with
`-DCUMETALC=/absolute/path/to/cumetalc` at configure time. If needed, also set
`-DCUMETAL_INCLUDE_DIR=/path/to/cumetal/include`. cuMetal 0.8 needs the
explicit include path shown above during device compilation. No GCC, TBB,
HIP, NVIDIA toolkit, or dependency download is needed by this CMake build.
The default top-level build still builds only the ML code.

## Add an example

Create `src/your_example.cu` with a kernel and its own `main()`, then run
`cmake --build build --target gustave` from the repository root. `check.cuh` is a tiny
optional helper for reporting CUDA errors.

These custom CMake commands do not produce `compile_commands.json`.
For clangd, use CUDA language mode and your cuMetal installation's CUDA
compatibility include directory; the previous GCC-specific config is removed.
