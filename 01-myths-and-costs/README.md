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

### Generic Types

The previous example used the std::array standard library container, a class
template that wraps a C-style array together with its size information. When a
std::array is used with a specific underlying type and size, the compiler
defines a new type during istantiation. `std::array<int, 10>` creates a
container type whose underlying C-style array holds 10 integers, while
`std::array<int, 20>` creates a container type whose underlying array holds 20
integers. These two are different types, they share the same underlying type but
differ in size. `std::array<float, 10>` would yeld a third type. Therefore,
different parameters, produce different types, and template types are generic
types that become concrete only upon instatiation.

## Ring Buffer

The ring buffer makes the case for generic types concrete, because the same
container has to be written three times in C before it does what one C++
template does. In a first example [rb_c_typec.c](examples/rb_c_typed.c), a
struct holds an int arr[BUFFER_SIZE] with write index and read index, and count,
it is simple and safe, but it only works for int. The second example,
[rb_c_void.c](examples/rb_c_void.c), erases the type by storing raw bytes and an
elem_size, copying values in and out with memcpy. One implementation now,
servers for every type, at the price of all type safety (pushing a float into a
buffer initialized with sizeof(int) compiles cleanly and the compiler says
nothing). The third, [rb_c_macro.c](examples/rb_c_macro.c), uses the
token-pasting operator to generate a ring_buffer_int_5 type together with its
init, push, and pop functions. That version is finally type-safe and
size-parametric, which is what was wanted, but the cost is paid in readability,
the caller must know what the macro-expands before calling anything.

```c
DECLARE_RING_BUFFER(int, 5)

ring_buffer_int_5 rb;
ring_buffer_init_int_5(&rb);
ring_buffer_push_int_5(&rb, i);
```

```cpp
ring_buffer<int, 5> rb;
rb.push(i);

while (!rb.is_empty())
{
    printf("%d ", rb.pop());
}
```

The C++ version in [rb_cpp.cpp](examples/rb_cpp.cpp) example, show the same
guarantees that macro, an it is more readble and easier to use. The ring_buffer
template can be instatiaded with intenger, float, or any other type and any
size, so `ring_buffer<int, 5>` and `ring_buffer<float, 3>` coexist without a
new implementation. The same push-and-pop logic is written once and applies to
every instantiation. which means the Dont Repeat Yourself (DRY) principle can be
applied across different types.

Templates are also used for template metaprogramming (TMP), a technique in which
the compiler uses a template to generate temporary source code, merges it with
the rest of the source, and compiles the result.

## constexpr

C++11 introduced the constexpr specifier, which declares that the value of a
function or a variable can be evaluated at compile time, so it must be
initialized immediately and have a literal type, as in a "constexpr double pi =
3.14159;". It is the preferred way of declaring compile-time constants,
replacing the C-Style macro approach, as we can se in the
[ohms_law_macro.c](examples/ohms_law_macro.c) example, with VOLTAGE and CURRENT
defined, computing const float resistance will print 3.00 (should be 3.30),
because both macros are parsed as integer literals and so is the division
(floating-point literals need the f suffix, ommited in the example). Rewriting
them in [ohms_law_constexpr.c](examples/ohms_law_constexpr.cpp) example with
constexpr will give resistance = 3.30, since the specifier allows the type of
each constant to be stated. Compile-time constants are therefore safer and
easier to read than macro constants.

The specifier also can be used to hint that a function can be evalueted at
compile time, which requires a literal return type, literal parameters, and, for
non-constructors, precisaly one return statement. In the code
[square_runtime.s](examples/square_runtime.s) we can see the assembly of a plain
`int square(int a)` called as square(2) (compiled in [compiler
explorer](https://godbolt.org/)) with arm gcc none and no optimization). So we
can see that the function manipulates the stack pointer, calls the function,
stores the value returned in r0, reloads it into r3, and moves it back to r0
(the ARM calling convetion's register to return values). Those unecessary
operations increase binary size and affect performance, and since the code is
valid C and valid C++, both compilers produce the same output. Marking the
function as `constexpr int ret = square(2);` changes the code generated to
[square_constexpr.s](examples/square_constexpr.s), so there is no square
function at all, only the value 4 that the compiler already computed. Heavy
computation can thus move from runtime to compile time whenever all parameters
are known.

## Bloat and runtime overhead

One common myth about C++ is that it produces bloated code and adds runtime overhead.

### Contructors and Destructors

Objects in C++ are instances of classes, variables that occupy memory and are
created by special functions called constructors. Constructors initialize
objects, including the initialization of class members, while destructors clean
up resources, and both are tied to an object lifecycle (the object is created by
a constructor and the destructor is called when the object goes out of scope).
Both also increase binary size and add runtime overhead, since executing them
takes time. We can se a first example of this in
[trivial_class_o0.s](examples/trivial_class_O0.s), where we have a private
number, a constructor that sets it, an empty destructor, and a getter,
instantiated once in main. Compiled with no no optimization enabled, this
simples abstraction produces 59 lines of assembly code (labels for each function
and a large number of instructions). This is the exactly kind of bloat code that
is undesirable in our embedded code.

We can observe that the destructor does nothing useful, so we can remove it,
then we will reduce the code to 44 lines, with no destructor code and no call to
it. With this we can learn that **you don't pay for what you don't use**, which
is one of the design principles of C++. As one second observation, is that C++
is not an OOP language but a multiparagigm (procedural, object-oriented, generic
and even functional). Private members settable throgh constructors have a price,
since structs in C++ have public members for default, we can rewrite the
previous code as [trivial_struct.s](examples/trivial_struct.s), we can drop the
assembly to 12 lines. Here we can say two thing, we don't pay for what we don't
use, and **using C++ does not bind the developer to an OPP paragim**.

Until now, all the examples were compiled with optimizations disabled, which is
why the assembly contains unnecessary operations that could be removed. We can
compile the first class example with optimization level O3, so we will get this
[trivial_class_O3.s](examples/trivial_class_O3.s), as we can see it reduced the
whole program to two instructions. The value of num obj is placed in r0 as the
return value, and everything else disappears. All the stack manipulation is
stripped away. Removing unnecessary instructions is therefore the job of the
optimization process rather than something the programmer must hand-tune. Even
so, optimization is often avoided in embedded projects, based on the claim that
it breaks code.

### Optimization

Unoptimized code results in unnecessary instructions that affect both binary
size and performance, yet many embedded projects are still built with
optimization disabled because developers do not trust the compiler and fear it
will break the program. There is come truth to the fear, but it only
materializes when the program is not well formed (when it contains undefined
behavior). One of the best-known examples of undefined behavior is signed
interger overflow, the standard does not define what happens when 1 is added to
the maximum value of a signed integer on the target platform, and the program is
not required to do anything meaningful in that case. The behavior can be
demonstrated with a small function that takes an integer, adds 1 to it, and
returns whether the result is greater than the original value:

```c
int foo(int x)
{
    int y = x + 1;
    return y > x;
}
```

Compile with GCC for both x86 and Arm Cortex-M4, the results are the same.
Without optimization, passing the maximum value, the function returns 0 and the
program reports that X is not larger than X+1, the compiler performs the integer
overflow, though the standard does not specify this and the behavior depends on
the compiler. With optimization enabled, the function returns 1 and the program
reports the opposite, because the generated code performs no calculation at all,
the compiler assumes the program is well formed and free of undefined behavior.

Compiler bugs that break functionality only under optimization are rare but not
unheard of, which is why unit and integration testing exists to validate
behavior in both builds. Optimization is essential for C++ abstractions while
keeping the binary footprint minimal and performance maximum, the highest level
(-O3) is used throughout this repo.

### Templates
