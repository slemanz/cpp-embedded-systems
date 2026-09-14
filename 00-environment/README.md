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
analysis and profiling with performance analysis.