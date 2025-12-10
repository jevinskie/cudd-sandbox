#include <cudd-sandbox/pla_file.hpp>

#include <optional>

#include <ctre.hpp>

std::optional<std::string_view> extract_number(std::string_view s) noexcept {
    using namespace ctre::literals;
    if (auto m = ctre::match<"^[a-z]++([0-9]++)$">(s)) {
        return m.get<1>().to_view();
    } else {
        return std::nullopt;
    }
}

SOP read_pla_file(std::string_view pla_path) {
    SOP pla;
    return pla;
}
