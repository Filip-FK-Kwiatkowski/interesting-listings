# Utilities

## FWD

`FWD()` replaces `std::forward()` usages.

## LIFT

`LIFT()` *lifts* a callable name to a level of a higher order function. A *name* that usually cannot be used passed as an argument (for example because it's a name of a function template or a set of overloads) can be lifted so that it can be used in those contexts.
