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
