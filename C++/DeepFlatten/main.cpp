#include <ranges>
#include <vector>
#include <set>
#include <utility>

#include <fmt/ranges.h>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LIFT(x) [](auto&&... args)  \
noexcept(noexcept(x(FWD(args)...))) \
-> decltype(x(FWD(args)...))        \
{                                   \
    return x(FWD(args)...);         \
}

auto deepFlatten(std::ranges::range auto&& rng) -> auto
{
    using RangeElementType
            = std::ranges::range_value_t<decltype(rng)>;

    if constexpr (std::ranges::range<RangeElementType>)
    {
        return rng
               | std::views::transform(LIFT(deepFlatten))
               | std::views::join;
    }
    else
    {
        return std::views::all(rng);
    }
}

auto main() -> int
{
    auto flat = std::vector{1, 2, 3};

    auto deep = std::vector <std::set <int>>{
        {1, 2}, {5, 3, 4}, {6}
    };

    auto deeper = std::set <std::vector <std::set <int>>>{
        {{1, 2}, {3}, {4, 5, 6}},
        {{7}, {8, 9, 10}}
    };

    fmt::println("{}", deepFlatten(flat));
    fmt::println("{}", deepFlatten(deep));
    fmt::println("{}", deepFlatten(deeper));
}
