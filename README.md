# Overview

Just enable the required features from the `CMakeLists` (@line 105) and it'll add all the required submodules and linker tags and even copy the DLLs to the binary directory on windows!

> Only works in `Release` or `RelWithDebInfo` modes on windows as the debug sdl dlls are a pain to move.

## Requirements (for compiling)
- [CMake](https://cmake.org/)
- x86 or x64 compiler (only tested with `MSVC`, `MinGW` on windows and `GCC` for UNIX)

## Profiling

You can use https://ui.perfetto.dev/ to view the profiling data. The data is stored in `profiling/` and is named `perfetto_trace.json`.
