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

Instantiating templates with different parameters causes the compiler to
generate distinct types, which increases binary size, an expected outcome, and
the same situation arises with generic ring buffer implemented in C using the
token-pasting operator and macros as we already seen. The price in binary size
is paid either way. And generally the difference in creating the types in C
style and using templates will be a few bytes greater in templates.

But it's important to say that templates do affect build time, because concrete
types are instantiated in different compilation units, though techniques exist
to avoid this when needed. All functions related to instantiated types sharing
the same parameters collapse into a single function in the binary, since the
liker removes duplicate symbols.

### RTTI and exceptions

Runtime type information (RTTI) is a mechanism that allows the type of an object
to be determined at runtime. Most compilers implement RTTI using virtual tables,
so every polymorphic class (a class with at least one virtual function) has a
vtable that among other things, carries the type information used for runtime
type identification. This imposes both time and space costs, increasing binary
size and affecting runtime performance whenever type identification is used,
which is why compilers provide a way of disabling it. We can see the example in
[rtti_dynamic_cast.cpp](examples/rtti_dynamic_cast.cpp), a Base struct with a
virtual print, a Derived struct that overrides it, and a printer that takes a
reference to Base, called once with each object.

The program prints Base, and then Derived, because classes with virtual
functions use their vtables for dynamic dispatch, the process of selecting which
implementation of polymorphic function is executed. Depending on whether a Base
or a Derived reference is passed to printer, dispatching selects the
corresponding print method. Since vtables also store type information,
dynamic_cast can recover the concrete type from a reference or pointer to the
superclass, and adding such a cast inside a printer makes the program report
that the type was found through RTTI. In GCC, RTTI is disabled with the
`-fno-rtti` flag, and compiling that will fail. RTTI is useful in certain
scenarios, but it adds massive overhead on resource-constrained devices, so we
must leave disabled.

Exceptions are another C++ feature often disabled in embedded code, they are an
error-handling mechanism based on try-catch block, we can see this in the
example [exceptions_unwinding.cpp](examples/exceptions_unwinding.cpp), with two
struct A and B, that print a message in their constructor and destructor. A
function bar creates a local b and throws, while foo creates a local a and calls
bar, and would create a second object afterwards. Calling foo inside a try block
produces the sequence: A created, B created, B destroyed, A destroyed, and only
then the catch block executed. This is what we call as **stack unwinding**, and
to make it happen standard implementation most commonly rely on unwind tables,
which store information about catch handlers, destructors to be called, an so
on. These tables can grow large and complex, increasing the memory footprint of
the application and introducing non-determinism because of the runtime mechanism
used for exception handling.

## Embedded Systems with Limited Resources

Embedded systems are specialized computing systems built for a specific use with
a limited set of responsabilities, in contrast to general-purpose computing
systems. The boundary between the two categories is not always clear, the system
that controls a toaster or a pup in an airplane is clearly an embedded systems,
and cellphones and early smartphones were also considered embedded systems, but
modern smartphones are closer to general-purpose computing devices. Our focus is
firmware development using modern C++ on small, resource-constrained embedded
systems. These systems are often employed in safety-critical applications, where
they are responsible for controlling a process in a timely manner, and they
cannot fail, since a failure can mean the loss of human lives.

### Safety-critical and Real-time

Safety-critical embedded systems often impose hard-real time requirements,
meaning that any missed deadline results in system failure. An Airbag Control
Unit (ACU) illustrates this, it collects data from accelerometes and presure
sensors, runs an algorithm that processes the data to detect side, front, and
rear-end crashes, and then controls the deployment of restraint systems such as
airbags and seat belt tensioners. ACU implementations must be resilient to
malfunctioning sensors and electronics, which is handled through redudant
sensors, comparison of data between sensors, comparison against thresholds, and
self-tests. Most importantly, an ACU has only a couple of milliseconds to
collect data, make decisions, and initiate deployment. It fails not only when it
does not detect a crash on time, but also when it deploys restraint systems
slightly too late, because a late deployment can harm the driver and passengers
more than no deployment at all. For this reason, an ACU must meet hard-real time
requirements, and for firmware this means that all worst-case execution must be
predictable.

Sensor and electronics faults are mitigated by redundancy, data sanity checks,
cross-comparison, and startup and runtime self-tests, which puts additional
stress on firmware. Algorithm failures, such as bad underlying model, fall
outside firmware responsibilities, the firmware job is to feed the algorithm
sensor data on time, execute it within a set time window, and act on its output.

### Measuring firmware performance and non-determinism

Ensuring that firmware meets its real-time requirements depends on measurement.
Useful metrics include performance profiling, which shows in which functions the
program spends the most time, response to external events, which shows how long
the system takes to react to an interrupt or a message on a communication bus,
and A-B timing, the most important metric for real-time requirements. A-B timing
measures how long firmware takes to execute from point A to point B, which is
not necessarily a function's duration, and this can vary with system state and
inputs. The simplest method is toggling an IO and measuring the interval with an
osciloscope, but it does not scale, since it requires an IO per function or
measuring one function at time. A microcontroller timer with ouput over UART
allows precise measurement but ties up a general-purpose timer. For this reason,
most microcontrollers have a Data Watchpoint and Trace (DWT) unit, which
supports program counter (PC) sampling and cycle counting and outputs events
through ITM unit. ITM can also output printf-style data from the firmware,
buffering it and sending it to an ITM sink such as SWO.

These units enable both profiling and instrumentation. DWT can periodically
sample the PC and send the samples through ITM over SWO to a host machine, where
the firmware's linker map file is used to generate the distribution of time
spent in each function. This shows where the program spends most of its time
without direct software instrumentation beyond setting the DWT and ITM, but it
is not particularly useful for A-B timing. For that, the GNU Compiler Collection
(GCC) offers the `-fintrument-functions` flag which inserts calls to
`__cyg_profile_func_enter` and `__cyg_profile_func_exit` at each function's
entry an exit. These functions, declared with extern "C", so the compiler are
able to link, can print the functions address and the DWT cycle count with
redirect to ITM printf, enabling A-B timing analysis on the host. Alternatively,
they can send ITM timestamps with function addresses, and the linker map file
then reconstructs the sequence of calls and returns.

A-B timing measuring can also reveal whether a function is deterministic. **A
function is deterministic when it produces the same duration and output for the
same inputs, and non deterministic when it depends on a global state and its
measured duration varies for the same inputs**. Default dynamic memory
allocators in C++ tend to be non-deterministic, because allocation time depends
on the allocator's current global states and the complexity of its algorithm.
Durations can be measured for the same inputs under different global states, but
evaluating all possible states and guaranteeing the Wort-Case Execution Time
(WCET) with default allocators is hard. Non-determinism is not the only problem
for safety-critical system, allocation can also fail when no memory is available
or when memory is fragmente. This is why safety coding standards such as Motor
Industry Software Reliability Association (MISRA) and Automotive Open System
Architecture (AUTOSAR) discourage dynamic memory.

### Dynamic memory management

Memory fragmentation is one way that dinamic allocation can fail, even with
plenty of free memory, a request for some size of units can return NULL, because
of no contigouos blocks can not be available. Beyond the non-dterministic
behavior of default allocators, out-of-memory scenarios are a major concern for
safety-critical system, and MISRA and AUTOSAR provive docing guidelines for
using C++ in such systems. 

MISRA is an organization formed by vehicle manufacturers, component suppliers,
and engineering consultancies that produces guidelines for automotive electronic
components, and its standards are also used in aerospace, defense, space,
medical, suppliers and companies from the electronics, semiconductor and
software industries. MISRA C++ 2008, which covers C++03, prohibits dynamic
memory allocation. AUTOSAR Guidelines for the use of the C++14 language in
critical and safety-related systems instead specifies a rule, which requires
memory management functions to ensure deterministic behavior with an existing
worst-case execution time, avoid fragmentation, avoid running out of memory,
avoid mismatched allocations or deallocations, and not depend on
non-deterministic kernel calls, and other rule which requires an analysis of
these same failure modes.

Following these rules to the letter is extremely hard. A custom allocator can
have a deterministic WCET and minimize fragmentation, but avoiding out-of-memory
conditions would require either verifying every allocation and mitigating
failures, or accurately estimating the memory needed so it never runs out at
runtime under any circustances. This adds more complexity than the value gained
by allowing dynamic allocation at startup but not while the system is running,
which is the approach of the Joint Strike Fighter Air Vehicle C++ Conding
Standard. MISRA C++ 2023 also advises against runtime allocation and recommends
startup allocation as a mitigation. The issue matters because the C++ standard
library uses dynamic allocation heavily, and exception handling implementation
often do as well. 

Containers in the C++ standard library allocate memory as they grow, and
std::vector is a representative case. A vector stores its elements contigously,
and its usual strategy is to allocate a single element on the first insertion
and double the capacity each time it is reached. When its memory is full, it
requires a block twice the size of the current one, copies the data into the new
storage, and deletes the previous block, so a growing vector produces a sequence
of allocations and deallocations, with the same behavior under GCC x86_64 and
Arm Cortex-M4. These requests can be monitored on any platform by overloading
the global new and delete operators, and the same overloading principle replace
the allocation mechanism globally to meet the guidelines on deterministic WCET
and out-of-memory scenarios, although doing so is quite challenging. When the
number of elements is known beforehand, the reserve method requests all the
memory at once, and more is requested only if the amount is exceeded. This fits
a policy that allows allocation at startup, as long as the element count is
guaranteed to stay within the reserved memory.

The standard library also supports local allocators, since the second template
parameter of a container is its allocator, which defaults to std::allocator,
based on new and delete. C++17 introduced `std::pmr::polymorphic_allocator`,
whose behavior depends on the `std::pmr::memory_resource` it is constructed
from, and `std::pmr::vector` is simply `std::vector` using this allocator. One
such resource, `std::pmr::monotonic_buffer_resource`, is built for performance,
releases memory only when destroyed, and can be initialized with a statically
allocated buffer, which makes it suitable for embedded applications. Each
request is served from the remaining space in the buffer, but once the buffer
cannot satisfy a request, the resource falls back to its upstream memory
resource, which by default uses new and delete. Replacing those operators
globally is even more challenging that it seems, because the standard library
defines several versions of them, and which one a container uses is hard to tell
without inspection. For this reason, a local allocator is usually the better
choice.

### Disabling unwanted C++ features
