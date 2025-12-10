#include <cudd-sandbox/pla_file.hpp>

#include <optional>
#include <utility>

#include <ctre.hpp>

std::optional<std::pair<std::string_view, std::string_view>> extr_implicant(std::string_view s) noexcept {
    using namespace ctre::literals;
    if (auto m = ctre::match<R"re(^\s*([01\-]+)\s+([01~]+)\s*$)re">(s)) {
        return std::make_pair(m.get<1>().to_view(), m.get<2>().to_view());
    } else {
        return std::nullopt;
    }
}

SOP read_pla_file(const std::string &pla_path) {
    SOP pla;
    return pla;
}
