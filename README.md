# expedition-gpu

A collation of my forays into the realm of GPU programming. Structured as
a mini monorepo: each subdirectory is a self-contained project with its
own build, dependencies, and README.

## Projects

- [`gustave/`](./gustave) — HIP-CPU based exploration (CPU emulator for
  AMD's HIP runtime; the API is essentially CUDA, so kernels translate
  with minimal changes).
