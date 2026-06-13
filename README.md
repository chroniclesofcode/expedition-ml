# expedition-ml

An exploration of machine learning and GPU programming, written from
scratch in C++. Structured as a mini monorepo: each subdirectory is a
self-contained area with its own purpose.

## Layout

```
expedition-ml/
├── CMakeLists.txt        # builds the ML trio (lune + maelle + verso)
├── lune/                 # primitives library  (MLP, attention, ...)
├── maelle/               # model architectures (Transformer, nanoGPT, ...)
├── verso/                # experiments / training scripts
└── gustave/              # GPU programming sandbox (HIP-CPU, self-contained)
```

The **ML trio** (`lune`, `maelle`, `verso`) is built together from the
top-level `CMakeLists.txt`. `gustave/` has its own build because it pulls
in HIP-CPU and we don't want that dependency on the ML side.

### Roles

- **`lune`** — primitives. Things every model needs: MLP, attention,
  layernorm, activations, optimizers, tensor utilities. Built as a static
  library; everything else imports from here.
- **`maelle`** — clean, importable model architectures composed from
  `lune` primitives. Vanilla Transformer, nanoGPT, etc. live here once
  they're settled.
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

This builds `liblune.a`, `libmaelle.a`, and all executables under
`verso/`. To run the starter experiment:

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

1. Header: `lune/include/lune/your_thing.hpp` — declare under
   `namespace lune`.
2. Source: `lune/src/your_thing.cpp`.
3. In `lune/CMakeLists.txt`, append the `.cpp` to the `add_library(lune ...)`
   source list. Headers are auto-discovered via the public include dir.

Consumers then `#include "lune/your_thing.hpp"`.

### Add a model to `maelle`

1. Header: `maelle/include/maelle/your_model.hpp` — declare under
   `namespace maelle`. Use `lune` primitives freely.
2. Source: `maelle/src/your_model.cpp`.
3. In `maelle/CMakeLists.txt`, append the `.cpp` to the `add_library(maelle ...)`
   source list.

`maelle` already links to `lune` PUBLICly, so headers from both
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

GPU programming exploration via HIP-CPU. Self-contained build; see
[`gustave/README.md`](./gustave/README.md).
