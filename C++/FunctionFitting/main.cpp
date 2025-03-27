#include <algorithm>
#include <concepts>
#include <functional>
#include <print>
#include <ranges>
#include <tuple>
#include <vector>

// ====================================================================================================

#define MAKE_REMOVE_MEM_FN_CV_SPEC(...) \
    template <class_type Class, typename Ret, typename... Args> \
    struct remove_mem_fn_cv<auto (Class::*)(Args...) __VA_ARGS__ -> Ret> { \
        using type = auto (Class::*)(Args...) -> Ret; \
    };

template <typename T>
concept member_function = std::is_member_function_pointer_v<T>;

template <typename T>
concept class_type = std::is_class_v<T>;

template <typename T>
concept functor = requires
{
    { class_type<T> };
    { &T::operator() };
};

template <typename>
struct remove_mem_fn_cv { };

MAKE_REMOVE_MEM_FN_CV_SPEC()

MAKE_REMOVE_MEM_FN_CV_SPEC(const)

MAKE_REMOVE_MEM_FN_CV_SPEC(volatile)

MAKE_REMOVE_MEM_FN_CV_SPEC(const volatile)

MAKE_REMOVE_MEM_FN_CV_SPEC(const&)

MAKE_REMOVE_MEM_FN_CV_SPEC(volatile&)

MAKE_REMOVE_MEM_FN_CV_SPEC(const volatile&)

MAKE_REMOVE_MEM_FN_CV_SPEC(const&&)

MAKE_REMOVE_MEM_FN_CV_SPEC(volatile&&)

MAKE_REMOVE_MEM_FN_CV_SPEC(const volatile&&)

namespace detail
{
    template <typename Func, typename Ret, typename... Args>
    consteval auto mem_fn_argc(auto (Func::*)(Args...) -> Ret) noexcept -> std::size_t
    {
        return sizeof...(Args);
    }
} // namespace detail

consteval auto argc(member_function auto mem_fn_ptr) noexcept -> std::size_t
{
    using stripped_mem_fn = remove_mem_fn_cv<decltype(mem_fn_ptr)>::type;

    return detail::mem_fn_argc(stripped_mem_fn{});
}

template <functor Functor>
consteval auto argc() noexcept -> std::size_t
{
    // formally illegal for things from standard library (e.g., std::function), because they aren't *addressable*
    return argc(&Functor::operator());
}

template <typename R, typename... Ts>
    requires (sizeof...(Ts) > 0)
consteval auto argc() noexcept -> std::size_t
{
    return sizeof...(Ts);
}

template <typename T>
struct freeArgc { };

template <typename R, typename... Args>
struct freeArgc<R(Args...)>
{
    static constexpr auto value = sizeof...(Args);
};

// ====================================================================================================


/*
 * O(argc(inv) * range.size())
 *
 * Could be worse, but maybe can be better (O(range.size())?)
 */
template <std::ranges::bidirectional_range Range, typename Invocable>
auto chunkedApply(Range&& range, Invocable&& inv)
{
    // static constexpr auto InvocableArgsN = argc<std::remove_cvref_t<Invocable>>();
    static constexpr auto InvocableArgsN = []() {
        using StrippedInvocable = std::remove_cvref_t<Invocable>;
        if constexpr (functor<StrippedInvocable>)
        {
            return argc<StrippedInvocable>();
        } else
        {
            return freeArgc<StrippedInvocable>::value;
        }
    }();

    for (auto chunks = std::forward<Range>(range) | std::views::chunk(InvocableArgsN); auto&& chunk : chunks)
    {
        std::invoke(
            [&inv]<std::size_t... Is>(auto&& chunk, std::index_sequence<Is...>) mutable -> decltype(auto) {
                auto const nextOrLast = [&chunk](std::size_t const index) {
                    auto advanced = std::ranges::next(chunk.begin(), index, chunk.end());
                    if (advanced == chunk.end()) --advanced;
                    return *advanced;
                };
                inv(nextOrLast(Is)...);
            },
            std::move(chunk),
            std::make_index_sequence<InvocableArgsN>()
        );
    }
}

auto f(int const a, int const b) {
    std::print("[{} {}]", a, b);
}

auto main() -> int
{
    auto const ofSize5 = std::vector<int>{1, 2, 3, 4, 5};
    auto const ofSize7 = std::vector<int>{1, 2, 3, 4, 5, 6, 7};

    chunkedApply(ofSize5, f);

    std::println("");

    chunkedApply(ofSize7, [](int const a, int const b, int const c) {
        std::print("[{} {} {}]", a, b, c);
    });

    auto const ofSize8 = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8};

    chunkedApply(ofSize8, [](int const a, int const b, int const c) {
        std::print("[{} {} {}]", a, b, c);
    });
}
