# Kotlin characteristics that annoy me

1. `companion`s. Should've stuck with `static`.
2. Function type inconsistencies. Type for variables is `(args...) -> ReturnType`, but for function signatures we have `(args...): ReturnType`.
3. `booleanThatCanBeTrue` and `booleanThatCanBeTrue == true` may behave differently if `booleanThatCanBeTrue` is of a `Boolean?` instead of a `Boolean` type. Maybe it should've been called `nullableBooleanThatCouldBeTrue`.
    - This also means that `nullableBooleanThatCouldBeTrue == true` and `nullableBooleanThatCouldBeTrue != false` **can be different**. `null == true` is `false`, while `null != false` is `true`. Thank to AoC-2024, day 5 for that.
4. [SAM conversions](https://kotlinlang.org/docs/fun-interfaces.html#sam-conversions) looked really weird the first time I encountered them (after moving from Java). I did not understand why you can't simply use a lambda there.
