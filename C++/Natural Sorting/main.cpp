#include <algorithm>
#include <charconv>
#include <functional>
#include <ranges>
#include <string_view>
#include <vector>

#include <fmt/ranges.h>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LIFT(x) [](auto&&... args)  \
noexcept(noexcept(x(FWD(args)...))) \
-> decltype(x(FWD(args)...))        \
{                                   \
    return x(FWD(args)...);         \
}

struct NaturalComparator
{

public:

    auto operator()(std::string_view const left, std::string_view const right) const -> auto
    {
        auto const withNumbersSeparated = std::views::chunk_by(sameCategory);

        return std::ranges::lexicographical_compare(
                left | withNumbersSeparated,
                right | withNumbersSeparated,
                LIFT(compareTokenizedRanges)
        );
    }

private:

    static auto sameCategory(char const left, char const right)-> bool
    {
        return isDigit(left) == isDigit(right);
    };

    static auto isDigit(char c) -> bool
    {
        return c >= '0' and c <= '9';
    };

    static auto compareTokenizedRanges(auto&& leftRange, auto&& rightRange) -> bool
    {
        auto firstLeftElement  = *leftRange.begin();
        auto firstRightElement = *rightRange.begin();

        auto leftIsANumber  = isDigit(firstLeftElement);
        auto rightIsANumber = isDigit(firstRightElement);

        if (leftIsANumber and rightIsANumber)
        {
            return toNumber(leftRange) < toNumber(rightRange);
        }
        else
        {
            return std::ranges::lexicographical_compare(
                    leftRange, rightRange
            );
        }
    }

    static auto toNumber(auto&& range) -> int
    {
        auto number = int();
        std::from_chars(range.begin(), range.begin() + range.size(), number);
        return number;
    }
};
