.. title:: clang-tidy - misc-make-ref-check

misc-make-ref-check
===================

Finds functions that returns pointers of a derived class of "I".
Suggests to wrap the function call in make_ref.
It is meant to find function calls that returns pointers to objects with custom reference counting which are not wrapped in handler code, missing wrapper can result in memory leaks.

example:
the checker will find the call to getA() and suggest to wrap it with make_ref, resulting in make_ref(getA()).

class I {};

class A : public I {};
A* getA();
void foo() {
    getA();
}