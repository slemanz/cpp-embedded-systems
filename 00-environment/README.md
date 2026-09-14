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