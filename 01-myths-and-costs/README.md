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
