# expedition-ml

An exploration of machine learning and GPU programming, written from
scratch in C++. Structured as a mini monorepo: each subdirectory is a
self-contained area with its own purpose.

## Layout

```
expedition-ml/
├── CMakeLists.txt        # builds the ML trio; gustave target builds/runs CUDA
├── lune/                 # primitives library  (MLP, attention, ...)
├── maelle/               # model architectures (Transformer, nanoGPT, ...)
├── verso/                # experiments / training scripts
└── gustave/              # GPU programming sandbox (CUDA via cuMetal, self-contained)
```

The **ML trio** (`lune`, `maelle`, `verso`) is built together from the
top-level `CMakeLists.txt`. `gustave/` has its own build because it uses
cuMetal to run CUDA kernels on the Apple GPU.

### Roles

- **`lune`** — primitives. Things every model needs: MLP, attention,
  layernorm, activations, optimizers, tensor utilities. Header-only;
  everything else imports from here.
- **`maelle`** — clean, importable model architectures composed from
  `lune` primitives. Vanilla Transformer, nanoGPT, etc. live here once
  they're settled. Header-only.

`lune` and `maelle` are header-only (`.hpp`) libraries — implementations
live inline in the headers under `include/`, and CMake exposes each as an
`INTERFACE` target. There are no `.cpp` files to compile or source lists
to maintain; only `verso` executables get compiled.
- **`verso`** — the research playground. Training scripts, ablations,
  novel architecture sketches. Each experiment is its own executable.
  When something here matures, it graduates into `maelle`.

Reserved names (folders to add only when a real need shows up):
`sciel`, `monoco`, `esquie`, `golgra`, `renoir`.

## Build
From the repo root:

```bash
cmake -S . -B build
cmake --build build -j
```

This builds the executables under `verso/`; `lune` and `maelle` are
header-only libraries. To run the starter experiment:

```bash
./build/verso/train_transformer
```

Expected output:

```
hello from verso
  lune primitive: lune::MLP(16 -> 32)
  maelle model:   maelle::Transformer(d=128, layers=6, ffn=lune::MLP(128 -> 512))
```

`compile_commands.json` lands in `build/` for clangd.

## How to add code

### Add a primitive to `lune`

`lune` is header-only. Drop the header at
`lune/include/lune/your_thing.hpp`, declare under `namespace lune`, and put
the implementation inline in the header. No `.cpp`, no CMake edit — the
header is auto-discovered via the public include dir.

Consumers then `#include "lune/your_thing.hpp"`.

### Add a model to `maelle`

`maelle` is header-only too. Drop the header at
`maelle/include/maelle/your_model.hpp`, declare under `namespace maelle`,
and use `lune` primitives freely. Implementation goes inline in the header;
no `.cpp`, no CMake edit.

`maelle` links `lune` as an `INTERFACE` dependency, so headers from both
namespaces are available to anything that depends on `maelle`.

### Add an experiment to `verso`

1. Source: `verso/src/your_experiment.cpp` — must define `main()`.
2. In `verso/CMakeLists.txt`, add:

   ```cmake
   add_executable(your_experiment src/your_experiment.cpp)
   target_link_libraries(your_experiment PRIVATE maelle lune)
   ```

   Drop `maelle` if you only need raw primitives.

After any of the above, rebuild:

```bash
cmake --build build -j
```

You only need to re-run `cmake -S . -B build` when CMake files change in
ways CMake can't pick up automatically (new top-level targets, new
`find_package`, etc.). Editing existing source lists is fine — the build
re-globs on its own.

## `gustave` (separate)

CUDA programming on Apple Silicon via [cuMetal](https://github.com/Lulzx/cuda-metal).
After the usual `cmake -S . -B build`, build and run all CUDA examples with:

```bash
cmake --build build --target gustave
```

New `gustave/src/*.cu` files are discovered automatically; each needs its own
`main()`. See [`gustave/README.md`](./gustave/README.md) for prerequisites.

## Style guide

House style for everything under this repo. A [`.clang-format`](./.clang-format)
at the root encodes the formatting rules; run it before committing.

**Naming**

- **Types** (classes, structs, enums, type aliases) — `PascalCase`:
  `MLP`, `Transformer`, `LayerNorm`.
- **Functions and variables** — `snake_case`: `describe()`, `d_model`,
  `n_layers`.
- **Member variables** — `snake_case` with a trailing underscore:
  `in_dim_`, `d_model_`, `n_layers_`. Exception: plain data structs (POD
  aggregates with public members and no behavior) use bare `snake_case`
  with no trailing underscore.
- **Namespaces** — lowercase, one word: `lune`, `maelle`.
- **Constants / enum values** — `snake_case` like other variables; no
  `ALL_CAPS` macros unless it's an actual preprocessor macro.

**Formatting** (enforced by `.clang-format`)

- 4-space indentation, no tabs.
- Braces on their own line everywhere — functions, `if`/`else`,
  `for`/`while`, classes, namespaces. Empty bodies stay inline as `{}`.
- 100-column soft limit.
- `*` and `&` bind to the type: `int* p`, `const Tensor& t`.

**Headers**

- `lune` and `maelle` are header-only; put the implementation inline in the
  `.hpp`. Every header starts with `#pragma once`.
- Declare `lune` code under `namespace lune`, `maelle` code under
  `namespace maelle`, and close the namespace with a `// namespace xxx`
  comment.
