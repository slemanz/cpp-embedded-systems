# 01. Myths and Costs

The goal of any programming language is to carry out the process of converting
application-specific abstraction into code that can be transformed into machine
code.

The simplicity of C comes at a cost. Because the language places the burden of
correctness on the developer, common tasks such as manual memory management,
pointer arithmetic, and buffer handling become frequent sources of defects.
C also permits a large amount of undefinied behavior, meaning that the same
source code may behave differently across compilers and target architectures.
In embedded systems these characteristics can be costly.

C++ was created originally as an extesion of C, and it inherits its low-level
capabilities while adding the ability to express complex ideas directly in the
language. Where C requires explicit and detailed code to manage complexity, C++
offers abstractions such as classes, and templates that let application-specific
concepts be represent closer to the way developers think about them. The key
characteristics for embedded systems is that most of these abstractions cost
nothing at runtime, for example, templates are resolved in compile time, so the
generated code can be as efficient as the equivalent C implementation. So we can
keep alot of things, while writing code that is easier to reason about and to
reuse.

## A short history of C++

The origins of C++ comes from the mid-60s, when the SIMULA language introduced
classes and objects. Classes are abstractions that represent real-world concepts
concisely and make code more human-readeble, in embedded systems, UART, SPI, and
temperature sensors are commonly examples. Also was introduced hierarchical
relationships beetwen classes: a PT100 is a TemperatureSensor, while a
TemperatureSensor holds a member object called PidController. This become known
as object-oriented language (OOP).

The first idea of C++ was to take these general abstractions, meant to help
humans represent things and combine them with the low-level capabilities of C,
the best language for that purpose at the time, so that high-level abstraction
could be used efficiently and close enough to the hardware. Originally started
as "C with classes", but became a modern language that still provides direct
access to hardware and memory-mapped peripherals, while its powerful
abstractions allow expressive and highly modular code.

In embedded development C remains the language of choice, but C++ adoption has
grown steadily, offering classes, improved type safety, and compile-time
computation. Several reasons explains this, C++ is a more complex language,
which makes harder for beginners, C is easier to learn and lets new developers
contribute to a project faster (even if is simplicity makes complex logic in C
becomes too verbose, which results in larger code bases). The remaining barriers
are myths, that C++ is just "C with classes" that is unacceptable for safety
critical systems because of dynamic memory allocation in the standard library,
or it produces a lot of bloated code and adds space and time overhead.

## C with Classes

C++ is more than C with classes as we can see in this code in C and its version
in C++:

```c
#define N 20

int buffer[N];

for(int i = 0; i < N; i++)
{
    printf("%d ", buffer[i]);
}
```

```cpp
std::array<int, 20> buffer;

for(const auto& element : buffer)
{
    printf("%d ", element);
}
```

The C code defines a constant N to fix the buffer's size, then uses that same
constant both to declare and as stop condition of the indexed loop. The C++
translation is shorter, uses fewer words, and reads closer to English.
Range-based loops, introduced in C++ 11, remove the cognitive burden of carrying
the container's size into the loop condition.

### Generic types