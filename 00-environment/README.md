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

### Vendored IDE, GCC and Compiler Explorer

An alternative to commercial development environments are vendor-supported
environments based mostly on Eclipse together with GNU Compiler Collection (GCC)
tools and the GNU Project Denugger (GDB), with STM32CubeIDE by ST and MCUXpresso
by NXP. These tools are packed with code configurator UIs that generate C code
for GPIO configuration, clock setup, and peripheral driver initialization. Other
vendors, such as Nordic Semiconductor, opted for VS Code as the basis of their
IDE solution and provide plugins for GPIO configuration and debugging,
benefiting from plugins such as IntelliSense for code completion, parameter
information, and syntax highlighting, GCC itself is one of the most used C and
C++ compilers in general, free software, and the most popular choice for
non-critical applications that do not require a qualified compiler. Even GCC can
be qualified, however, the process involves compiling and running test programs
and comparing outputs against expected results, documenting every issue found,
and putting a process in place to mitigate them.

Besides the compiler, GCC, includes an assembler and a linker, exposed through a
driver program (gcc for C and g++ for C++), that runs preprocessing,
compilation, assembly, and linking in sequence. For a single file such as
main.cpp the preprocessor adds all header files specified with #include and
expands macros in the translation unit, the compiler turns that result into
assembly: the assembly stage produces an object file and the linker links it
with the C and C++ standard libraries to generate an ELF file. Intermediate
outputs can be inspected with additional arguments, such as the -E flag for
preprocessor output, which can also be explored in Compiler Explorer, an
interactive online compiler. In a hello world example using ARM GCC 11.2.1
(none), the preprocessor expands cstdio into 808 lines, and the assembly view
shows printf optimized into puts, whose body is absent because it comes from the
linked standard library.

Producing code that actually runs on a microcontroller additionaly requires
clock and peripheral initialization code, architecture and instruction-set
compiler flags, a startup assembly script with a reset handler and C and C++
runtime initialization, a linker script defining RAM and Flash memory regions,
and instructions to link against specific standard libraries. The resulting ELF
file is converted to binary or hex with objdump or flashing. From version 10,
GCC also has an integrated static analyzer, enabled with the -fanalyzer flag.

## Static analyzers

Static analyzers go through source code to detect potential issues such as
undefined behavior, or to check whether the code complies with a safety standard
such as MISRA or AUTOSAR, though capabilities vary only commercial versions
support safety standards checks. Typical detections include use of uninitialized
data, out-of-bounds array access, null pointer dereference, division by zero,
and memory management issues such as use after delete and double delete. GCC's
analyzer is enabled with the -fanalyzer flag: in a sum function taking a
`std::array<int, 4>` constant reference, failing to initialize the ret variable
leaves it populated with whataver occupies the allocated stack location, causing
undefined behavior, and the analyzer issues a warning that regular warnings such
as -Wall, -Wextra, and -Wpedantic do not catch.

The flag -Wuninintialized would warn only when compiled with an optimization level other than 0, such as -O2, while the clang compiler detects the same issue on its own (a reminder that compilers and analyzers differ in capability, so running code through several of them is good practice). 

A second example, accessing the fifth element of a four-element array, is also
caught by GCC's analyzer, and -Werror makes such warnings fail compilation so no
ELF file is generated; the trade-off is that static analysis takes more time,
which can matter in larger code bases. Other commonly used analyzers are
clang-tidy and cppcheck, both easy to install and use. Static analysis catches
common programming errors and enforces compliance, but it does not guarantee
that the code does what it is supposed to do, which requires manual tests on a
target or unit testing of individual pieces of code.

## Unit testing