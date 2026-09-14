# Environment

At the heart of every embedded system sits a microcontroller, and the landscape
is vast, ranging from cost-effective 8-bit and 16-bit cores to modern 32-bit Arm
and RISC-V based microcontrollers. This variety of architecture has impacted the
development of tools and compilers: some manufacturers focus on C support, while
many have recognized the importance of C++ and provided good support for it in
their toolchains. Since not all architectures and vendors can be covered, the
focus falls on Arm Cortex-M, a dominant architecture for modern microcontrollers
and SoCs, along with its development environments, static analyzers, unit
testing, and profiling.

## Compilers and Development Environments

Many vendors providing tools for embedded development offer functional safety
verstions of their tools and compilers, but free development enviroments and
open source compilers also exist and can be used in non-critical applications.
Development enviroments pack different tools together to make the development
process steamless, and those same tools can be used individually and tailored
to individual or orgazitional preferences. A code editor may be as basic as a
text editor or as advanced as Vim or Visual Studio Code, which support pluggins
for syntax highlighting, autocompletions, code navigation across source files.
and refactoring. The compiler and the linker transform code into object files
and link them into executable and binary files that can be flashed to a target.
Among the most popular C++ compilers are GCC, Clang, Arm Compiler for Embedded,
and IAR C/C++ compiler. The debugger flashes and debugs the target, and its
system consists of a debugger probe and the software that communicates with that
probe.

The remaining tools address building, analysis, and measurement. Build systems
such as GNU Make and Ninja control the process of compiling and linking, while
CMake and Bazel handle build automation and dependency management. Static
analysis tools analyze source code and, depending on their capability, detect
some forms of undefined behavior such as out-of-bound access, uninitialized
variables, and null pointer dereferences; dedicated static analysis tools can
also check whether the code is MISRA or AUTOSAR-compliant. Runtime profilers
combine target capabilities, software instrumentation, and debugger probes to
measure function execution time and analyze software performance. Most embedded
integrated development environments (IDEs) provide project creation and
organization, build automation, and debugging. Some development environments go
further and integrate more advanced features for code analysis, namely static
analysis and profiling with performance analysis.td::

### Arm Keil MDK and Arm Compiler for Embedded

Arm Keil MDK is a set of tools for embedded development on mostly Arm Cortex-M
microcontrollers, comprising Keil Studio (a set of extensions for VS Code), Keil
uVision (a legacy Windows-based IDE), Arm Compiler for Embedded, and Arm Virtual
Hardware. Both IDEs cover project configuration for different targets, build,
and debugging on target; uVision integrates PC-Lint for static analysis, a
simulator, and a profiler within its debugger, while Keil Studio can be
configured to use clang-tidy or cppcheck.

Arm Virtual Hardware Fixed Virtual Platforms runs binaries on simulated targets
in the cloud, providing infrastructure for CI/CD. MDK comes in a non-commercial
Community version and the commercial Essential and Professional editions, with
only Professional offering functional safety support and the FuSa compiler
certified to IEC 61508, ISO 26262, EN 50128, and IEC 62304. The compiler
toolchain consists of armclang, an LLVM-based compiler, armlink, Arm C
libraries, and Arm C++ libraries based on LLVM libc++. Arm Compiler supports
C++17 and the FuSa 6.16 version supports C++14, showing how slowly commercial
compilers adopt the latest standards (C++20 and C++23 were already released)
which leaves the newest language features unavaients.

### IAR Compiler and Workbench

IAR Embedded Workbench, from the Swedish company, is a development environment
for Arm Cortex-M, Cortex-R, and Cortex-A cores that integrates an IDE with
debugger and profiler, the IAR C/C++ Compiler, the C-STAT static analyzer, and
C-RUN for runtime analysis. Beyond standard tools such as a debugger, the IDE
offers more advanced embedded capabilities, including profiling and running
firmware in a simulator. C-STAT can run static analysis against safety coding
standards such as MISRAC++2008, while C-RUN instruments the code to cover heap
checks, bounds checking, buffer overrun, integer overflow, and other runtime
checks. The compiler supports C++17 from version 9.30.1, and the FuSa version of
IAR Embedded Workbench for Arm 9.50.3, released in February 2024, also provides
C++17 support. Together with Arm Compiler for Embedded, IAR represents the
commercial option for embedded development, and the strength of both is that,
alongside commercial support, they provide safety-qualified versions for
safety-critical projects. Some microcontroller vendors instead provide their own
development environments, usually based on Eclipse, with additional support for
their own products.