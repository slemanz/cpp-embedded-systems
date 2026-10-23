# 03. Beyond Classes

C++ began as "C with classes", which makes classes one of the first concepts
developers with a C background learn, but the language offers other fundamentals
features that make it much more than that. Before moving on to more advanced
topics, we have to lear about namespaces, function overloading, interoperability
with C, references and standard library containers and algorithms.

## Namespaces

Namespaces in C++ are scope specifiers for accessing type names, functions, and
variables, and they make it easier to tell apart similar identifiers in large
code bases built from many software components. Where C relies on prefixes such
as hal_init() or hal_get_ms(), C++ groups related code logically inside a
namespace.

All members are accessible unqualified from within the namespace, while code
outside it uses the namespace name followed by the scope resolution operator
(::).

```cpp
namespace hal {
    void init();
    std::uint32_t get_ms();
}

hal::init();
std::uint32_t time_now = hal::get_ms();
```

C++ standard library types and functions are declared in the std namespace. The
"using" directive gives unqualified access either to a single identifier (using
std::array) or to a whole namespace (using namespace std), but it is recommended
to use it sparingly, especially with "std", limiting it to a narrow scope or,
better, to individual identifiers.

The same namespace can span several headers, as std::vector and std::array are
declared in different header files yet belong to the same logical group.
Anything not declared in an explicit namespace belongs to the global namespace,
and organizing all code in namespace is good practice, the only exception is
"main", which must be global.

A global identifier is accessed with "::" and no namespace name:

```cpp
const int ret_val = 0;

int main()
{
    return ::ret_val;
}
```

### Unnamed namespaces

A namespace declared without a name, an unnamed namespace, makes the functions
and types inside it local to the translation unit where they are declared,
giving them internal linkage so code from other translation units cannot access
them:

```cpp
namespace{
    constexpr std::size_t c_max_retries;
    std::size_t counter;
};
```

The same effect can be achieved in both C and C++ with the "static" storage
specifier.

### Nested namespaces

Namespaces can be nested to organize code hierarchically, such as a top-level
"sensors" namespace containing "environmental" and "indoor_air_quality", and
since C++17 a nested namespace can be declared in a single line:

```cpp
namespace sensors::environmental{
    class temperature {};
    class humidity {};
};
```

This makes code more readable, since identifiers stay short without C-style
prefixes.

## Function overloading

Besides functions with the same name in different classes, C++ allows functions
with the same in the same scope to take different parameters, which are called
overload functions. Instead of names like print_int and print_float, the same
name is used and the compiler picks the right function at the call site based on
the arguments passed:

```cpp
void print(int a)   { printf("Int %d\r\n", a); }
void print(float a) { printf("Float %2.f\r\n", a); }

print(2);   // int version
print(2.f)  // float version
```

To tell overloads apart, the compiler uses name mangling, encoding extra
information such as parameter types into function names so each overload gets a
unique symbol, as in "_Z5printi" and "_Z5printf" in the assembly output.
Overloaded functions may differ in the number of arguments, but not in return
type alone, since two functions with the same name and arguments are treated as
redeclaration and cause a compile error:

```cpp
int print(int a);
void print(int a); // error
```

Function overloading is a basic but powerful feature that provides compile-time,
or static, polymorphism.

The [function-overloading.cpp](examples/overloading/function-overloading.cpp) shows
a running example.

## Interoperability with C

Some of the previous examples in the repository run in renode simulator, they
mix C++ and C code, relying on the vendor-provided HAL library and Arm's Common
Microcontroller Software Interface Standard (CMSIS), both written in C and kept
in the platform folder. The "add_executable" function in CMakeList.txt lists
many C files from that folder alongside just a few C++ files, such as main.cpp
and uart_stm32.cpp. Each C and C++ file is translation unit built separately by
the C and C++ compilers respectively, and the resulting object files are then
linked into a single ELF file.

### External and Language Linkage in C++

Variables and functions with external linkage can be linked with code in other
translation unit as long as the compiler has access to their declarations, and
their language linkage property is what allows C++ to be linked with C code,
declared with the following syntax:

```cpp
extern "C" {
    void c_func();
}
```

A declaration with C language linkage follows C linkage conventions, which
prevents name mangling, among other things, so it links properly with code
compiled in a C translation unit.

### C standard library in C++

C++ wraps the C standard library in headers with a "c" prefix and no extension,
such as `<cstdlib>` for `<stdlib.h>`, and in GCC these wrappers include the
original C headers, which ensure C++ compability by adding a C language linkage
specifier when compiled by a C++ compiler, a practice also followed by vendor
HAL implementations such as the STM32F0xx HAL headers:

```cpp
#ifdef __cplusplus
extern "C" {
#endif

// declarations

#ifdef __cplusplus
}
#endif
```

Since C libraries are often used in C++ programs, especially in embedded
projects, it is always a good idea to guard them, and headers that lack internal
guards can be wrapped at the include site with `extern "C" { # include "c_library.h" }`.

## References

References are aliases for objects, so they must be initialized immediately, and
since they are not objects themselves, there are no pointers to references or
arrays of references. C++ has two types of references, lvalue references and
rvalue references.

### Value categories

C++ expressions belong to one of two value categories, lvalue or rvalue, a
simplified division with historical origins. Lvalues usually appear on the left
side of an assignment, though not always, and they have an addres the program
can access:

```cpp
int a = 42;     // a is lvalue
int b = a;      // a can also appear on the right side
int *p = &a;    // pointer p is lvalue
```

Rvalues usually appear on the right side of an assignment and can be thought of
as temporary values, such as literals, function calls that do not return
references, and built-in operator calls like "a+16" or "sizeof(int)". An rvalue
can still appear on the left side of an assignment, as in the following
expression, where a temporary "my_struct" created by the default constructor
receives a copy of another temporary built with the "int" constructor, printing
"a_ = 16" ([rvalue.cpp](examples/references/rvalue.cpp)):

```cpp
printf("a_ = %d\r\n", (my_struct()=my_struct(16)).a_);
```

### Lvalue references

Lvalue references are aliases for existing objects, declared by adding "&" to
the type name, and they can also be const-qualified, as seen in the
[lvalue-refs.cpp](examples/references/lvalue-refs.cpp) example. The object can
be manipulated through the reference, while any attempt to change the value
through a constant reference results in a compiler error.

### Rvalue references

Rvalue references, declared with "&&" next to the type name, extend the lifetime
of temporary rvalues and cannot be bound to lvalues, since doing so causes a
compiler error:

```cpp
int&& a = 42;
int b = 0;
// int&& b_ref = b; compiler error
int&& b_ref = b + 10; // ok, b + 10 is rvalue
```

They are important for resource management and are used in move semantics, which
allows resources to be moved from one object to another. std::vector illustrates
this with two "push_back" overloads in the
[rvalue-refs.cpp](examples/references/rvalue-refs.cpp) example. One taking
"const T&", which copies the value, and one taking "T&&", which moves it so the
new element takes ownership of the value's dynamically allocated resources.

```cpp
v.push_back(str);            // copy: str unchanged, new data address
v.push_back(std::move(str)); // move: str left valid but unspecified, often empty
```

std:move casts str to an rvalue reference, signaling that its resources can be
moved instead of copied. The printed addresses confirm it to us.

## Standard library containers and algorithms