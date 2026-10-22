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