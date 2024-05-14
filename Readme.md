# Lacam Integration to Pogema

This repository contains code for integrating the Lacam library into the Pogema environment. Lacam is a C++ library used for solving various pathfinding and planning problems. The integration allows you to run Lacam in the Python Pogema environment.


## Requirements

- CMake
- Make
- Python 3.x
- `pogema` environment
- `ctypes` module to load cpp Lacam library
- `IPython.display` for displaying results in Jupyter Notebook
- `PIL` and `matplotlib` for image handling (if needed)

## Build Instructions

To build the Lacam library, run the following commands:

```sh
cd lacam3
cmake -B build && make -C build
```

or use ```build_lacam.sh``` script:

```sh
./build_lacam.sh
```
