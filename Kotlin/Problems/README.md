# Kotlin characteristics that annoy me

1. `companion`s. Should've stuck with `static`.
2. Function type inconsistencies. Type for variables is `(args...) -> ReturnType`, but for function signatures we have `(args...): ReturnType`.
3. `BooleanThatCanBeTrue` and `BooleanThatCanBeTrue == true` may behave differently if `BooleanThatCanBeTrue` is of a `Boolean?` instead of a `Boolean` type.
