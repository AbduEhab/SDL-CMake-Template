# Overview

Just enable the required features from the `CMakeLists` (@line 105) and it'll add all the required submodules and linker tags and even copy the DLLs to the binary directory on windows!

> Now works in `Release` _and_ `RelWithDebInfo` modes!

## Requirements (for compiling)
- [CMake](https://cmake.org/)
- x86 or x64 compiler (only tested with `MSVC` and `MinGW` on windows, and with `GCC` for UNIX)

## Profiling

You can use https://ui.perfetto.dev/ to view the profiling data. The data is stored in `profiling/` and is named `perfetto_trace.json`.
