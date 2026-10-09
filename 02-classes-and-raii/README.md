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
