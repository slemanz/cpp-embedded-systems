# 02. Classes and RAII

Classes in C++ are a means of organizing code into logical units, acting as
blueprint that structure data together with the functions that operate on that
data. These blueprints are used to build instances known as objects, which can
be initialized with data, manipulated by calling functions or methods on them,
stored in containers, or passed by reference to objects of other classes so that
different parts of a system can interact.

For this reason classes are the basic building blocks of C++ applications, they
organize code into units with isolated responsability that reflect dependencies
and interactions with the rest of the system, and they can be combined or
extended to reuse functionality and adds capabilities. In embedded systems, they
serve as abstractions of both low-level components, such as UART Drivers and
libraries, and business logic components, such as a cellular modem library.

## Encapsulation

Encapsulation is a programming concept that organizes code into units containing
both data and the functions that operate on that data. It is not strictly tied
to Object-Oriented Programming (OOP) and appears in other paradigms well, where
it decouples code into units with single responsabilities, making it easier to
reason about, more readeble, and simpler to maintain. In OOP, encapsulation can
also mean hiding an object's members or restricting outside access to them,
which C++ achieves through the access specifiers public, private and protected.
Public and private are the most common, and they control the interface of a
class, that is, which members are available to it users, the
[encapsulation.cpp](examples/encapsulation/encapsulation.cpp) examples show
this.

Its public section holds a constructor that initializes the private member
variable baudrate_ and an init method that writes a value to the STM32-specific
register through the private write_brr method, with the value computed in the
private calculate_uartdiv method. Public methods can use private members, but
calling uart1.write_brr(5) from outside would fail to compile. Hiding methods
and data from the user of the class, defines a clear interface and protects both
the class and its user from unwanted behavior.

The init method exists alongside the constructor to give full control over
hardware initialization. An object may be constructed as a global or static
variable, and such objects are initialized before reaching main and before the
hardware is initialized. For this reason, some kind of init method is common in
embedded classes, ensuring that all hardware peripherals are initialized in the
correct order. Since the default access specifier for classes in C++ is private,
the uart class could also be written without an explicit private label.

```cpp
class uart {
    std::uint32_t baudrate_;
    std::uint8_t calculate_uartdiv();
    void write_brr(std::uint8_t);
public:
    uart(std::uint32_t baud = 9600);
    void init();
};
```

The original definition still declares the private section explicity and places
it after the public one, because the public members form the class's interface,
and that is what a reader wants to see first when learning how to interact with
the class. The only data member, baudrate_, is private and can be set only
through the constructor. When data members are meant to be exposed publicity,
the common practice is to define setter and getter methods for them.

### Setters and getters

For the uart class, trivial setters and getters for baudrate_ would expose the
value through the public interface, but they add no value, since they are
equivalent to placing baudrate_ under the public access specifier. Setters and
getters should serve a clear purpuse, such as a setter that includes validation
logic:

```cpp
void set_baudrate(std::uint32_t baudrate)
{
    if(baudrate <= c_max_baudrate)
    {
        baudrate_ = baudrate;
    }else
    {
        baudrate_ = c_max_baudrate;
    }
}
```

This setter performs a sanity check and assigns the private member only when the
value makes sense, otherwise falling back to the maximum baudrate supported by
the system, although changing the baudrate after UART initialization makes
little sense in practice. Exposing data members through setters and getters in
some sense breaks encapsulation, because its idea is to hide implementation
details, and data members are implementation details. For this reason, setters
and especially getters should be used sparingly and only when they serve as a
meaningful purpose. Classes can also encapsulate only functionality without
data, or data commonly shared by all users of a class, which is done through
static methods.

### Static methods

Static methods are C++ methods declared with the static keyword, and they are
accessible without object instantiation. Instead of creating a uart object
through a single-argument constructor that takes the baudrate and then calling
init on it, the uart class can be designed as a type whose methods are all
static: the constructor and the baudrate_ private data member are removed, and
the baudrate is passed directly from init to calculate_uartdiv, so the method is
called using the class name followed by a double colon:

```cpp
static void init(std::uint32_t baudrate)
{
    write_brr(calculate_uartdiv(baudrate));
}

// ...

uart::init(115200);
```

Static methods can only use static data members and other static functions of
the class, because non-static members require an object to be instatiated.
Functions could also be grouped into a common unit with namespaces, but grouping
the into a type is useful because types can be passed as template arguments. C++
also allows types to be defined with the struct keyword, whose members are
public by default. Historically, structs were used for compatibility with C,
allowing a header file for a library to be shared between C and C++ programs; in
that case, the shared struct could contain only common data types and no methods
as members.

### Structs

Structs are commonly used in C++ for types that have only data members meant to
be publicy available to users, and they are mostly identical to classes except
that their default access level is public. A typical example is a struct holding
the x, y, and z axis values of an accelerometer:

```cpp
struct accelerometer_data
{
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;
};
```

Such data could be produced by a sensor class, stored in a ring_buffer class,
and consumed by a sensor_fusion class, which means accelerometer_data acts only
as data holder while the behavior related to that data is implemented elsewhere.
Choosing between simple structs and classes with data and complex behavior is a
design decision that depends on the exact application. Structs are also used to
group functions into types, usually declaring them all as static and public, a
struct is convenient here because its default access specifier is public, and it
also reflects the intent, since structs are usually used when all members are
public. Besides public and private, C++ also has the protected access specifier,
which is related to inheritance.

## Storage duration and Initialization

C++ objects with automatic storage duration are initialized upon declaration and
destroyed when execution exits the variable scope, while objects can also have a
static storage duration. Data members of objects can likewise carry static
storage specifiers, and specific rules govern the initialization of such
members, which is why non-static member initialization is examined first.

### Non-static member initialization

Non-static class members can be initialized in different ways, and constructors
are the first mechanism that usually comes to mind, being powerful C++ features
that offer great control over initialization. Before turning to constructors,
however, the starting point is default member initializers.

#### Default member initializers

Since C++11, default member initializers allow a default to be set for a class
member directly in the class definition, something that would fail to compile
under any pre-C++11 standard:

```cpp
class my_class
{
    int a = 4;
    int *ptr = nullptr;
};
```

This improves readability and avoids setting the same member as variable
repeatedly when a class has multiple constructors, and it is particularly useful
for pointers. Without the default initializer, ptr would hold some random value
from memory, and dereferencing it would read from or write to a random location,
potentially causing a serious fault. A compiler or static analyzer would detect
this situation by reporting the use of an unintialized value, which is undefined
behavior, but is still shows the importance of giving member variables default
values, a task for which default member initializers are one option.

#### Constructors and member initializer lists

Constructors are nameless methods in a class definition that cannot be called
explicity and are invoked when an object is initialized. A constructor that can
be invoked with no arguments is called the default contructor, which includes
the uart constructor `uart(std::uint32_t baud = 9600): baudrate_(baud) {}`,
since, its default argument of 9600 is used when no argument is provided at the
call. Using it with `uart uart1;` is called default initialization, performed
when an object is declared with no initializer. Parentheses are omitted because
`uart uart1();` would be interpreted by the compiler as the declaration of a
function named uart1 that takes no arguments and returns a uart object. Since
the constructor also accepts an argument, direct initialization such as `uart
uart1(115200);` passes 115200 as the baud argument.

The baudrate_ member is initialized through the member initializer list,
specified after the colon and before the opening brace of the constructor body,
with multiple items separated by commas. Se the [member initializer
list](examples/initialization/member-initializer-list.cpp) example.

Here the sensor constructor initializes a reference to a uart object and a const
unsigned integer, read_interval_. References in C++ are similar to pointers in
C, pointing to na already-created object, but they must be initialized when
declared and cannot be reassigned to another object. For this reason, references
and const-qualified members must be initialized using a member initializer list.
Constructors can have zero or many parameters, and one with a single parameter
declared without the explicit specifier is called a converting constructor.

#### Converting constructors and explicit specifiers

Converting constructors allow the compiler to implicitly convert the type of
their argument into the type of their class. In the
[converting-constructors.cpp](examples/initialization/converting-constructors.cpp)
example the uart_consumer expects a uart object, yet calling
uart_consumer(115200) compiles because the compiler uses the converting
constructor to build a uart from 115200, so the program prints a baudrate of
9600 for the default-constructed uart1 and 115200 for the second call. Implicit
conversion can be unsafe and is often unwanted, and it can be prevented by
declaring the constructor with the explicit specifier.

```cpp
explicit uart(std::uint32_t baud = 9600): baudrate_(baud){};
```

With this change, the same call fails to compile with an error stating that
115200 cannot be converted from int to uart, which guarantees that no user of
the class creates a potential implicit conversion leading to unwanted behavior.

Calls with specific argument types can also be blocked, which is useful, for
example, when a constructor expects a uint8_t and someone calls it with a
uint32_t. A specific constructor can be deleted in the class declaration with
`uart(float) = delete`, so calling `uart uart1(10000.0f);` results in a compiler
error about the use of a deleted function. Another option is brace list
initialization, `uart uart1{10000.0f}`, which restricts narrowing and prevents
the float-to-integer conversion, producing a compile error about a narrowing
conversion. This means list initialization limits implicit conversion and helps
detect problems at compile time. Class data members can also be declared with
the static keyword, and their initialization follows special rules.

### Static member initialization

Static members are not tied to individual objects of a class or struct, they are
variables with static storage duration that any object of the class can access.
The object_counter in the
[static-member.cpp](examples/initialization/static-member.cpp) example
illustrates this with a single static data member, the integer cnt, which the
constructor increments and the destructor decrements. In main, three
object_counter objects are created inside an unnamed scope, and the value of cnt
is printed both inside that scope and after leaving it. Inside the scope, cnt
equals 3 because three objects exist, and once executions exits the scope, the
destructors are called and decrement ctn back to 0, which the program's output
confirms.

In this example, the static variable is declared in the class but defined
outside it with a separate line:

```cpp
int object_counter::cnt = 0;
```

Since C++17, a static variable can instead be declared with te inline specifier
inside the struct or class definition and given an initializer there, as in
`inline static int cnt = 0;`. This makes the code more concise and easier to
read, and it is easier to use because the variable no longer needs to be defined
outside the class definition.

## Inheritance and dynamic polymorphism