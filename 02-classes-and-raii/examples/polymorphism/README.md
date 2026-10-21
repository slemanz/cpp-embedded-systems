# Template

To build through CMAKE, use:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

or through makefile with, commands like:

```bash
make debug
make release
make run
make clean
```

Note: about the "cmake -S . -B", -S sets the source directory (where
CMakeLists.txt is) and -B build sets the build directory (where generated files
go).

To quit from simulation type "quit" or "q".