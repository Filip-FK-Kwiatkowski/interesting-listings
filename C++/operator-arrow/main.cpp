#include <iostream>

struct What {
    auto xd() -> void {
        std::cout << "wololo\n";
    }
} theFuck;

struct Outer {
    struct Inner {
        auto operator->() -> What* {
            return &theFuck;
        }
    };

    auto operator->() -> Inner {
        return {};
    }
};

auto main() -> int {
    auto o = Outer();
    o->xd();
}
