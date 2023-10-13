#include <utility>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LIFT(x) [](auto&&... args)  \
noexcept(noexcept(x(FWD(args)...))) \
-> decltype(x(FWD(args)...))        \
{                                   \
    return x(FWD(args)...);         \
}
