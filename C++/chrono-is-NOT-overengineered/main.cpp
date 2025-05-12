#include <chrono>
#include <print>

auto displayInWarsawAndNewYork(std::chrono::year_month_day const date) -> void;

auto main() -> int {
    using namespace std::chrono;

    displayInWarsawAndNewYork(20d / March / 2019);
    displayInWarsawAndNewYork(3d  / April / 2019);
}

auto displayInWarsawAndNewYork(std::chrono::year_month_day const date) -> void
{
    using namespace std::chrono;

    // 18:00 of that day in a given timezone
    auto const localDays = local_days(date) + 18h;
    auto const inWarsaw  = zoned_time("Europe/Warsaw", localDays);
    auto const inNewYork = zoned_time("America/New_York", inWarsaw);

    std::println("{}", inWarsaw);
    std::println("{}", inNewYork);
}
