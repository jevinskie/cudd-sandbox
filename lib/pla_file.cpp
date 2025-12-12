#include <cudd-sandbox/pla_file.hpp>

#include <cudd-sandbox/utils.hpp>

#include <charconv>
#include <cstddef>
#include <exception>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include <ctre.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

template <typename T> using opt = std::optional<T>;

static opt<Implicant> extr_implicant(std::string_view s) noexcept {
    if (auto m = ctre::match<R"re(^\s*([01\-]+)\s+([01~]+)\s*$)re">(s)) {
        return Implicant(m.get<1>().to_view(), m.get<2>().to_view());
    } else {
        return std::nullopt;
    }
}

static opt<std::vector<std::string>> extr_ilb(std::string_view s) noexcept {
    if (auto m = ctre::match<R"re(^\s*\.ilb\s+((?:\S+\s+)*\S+)\s*$)re">(s)) {
        std::vector<std::string> res;
        for (const auto label : ctre::split<R"re(\s+)re">(m.get<1>())) {
            res.push_back(label.get<0>().to_string());
        }
        return res;
    } else {
        return std::nullopt;
    }
}

static opt<std::vector<std::string>> extr_ob(std::string_view s) noexcept {
    if (auto m = ctre::match<R"re(^\s*\.ob\s+((?:\S+\s+)*\S+)\s*$)re">(s)) {
        std::vector<std::string> res;
        for (const auto label : ctre::split<R"re(\s+)re">(m.get<1>())) {
            res.push_back(label.get<0>().to_string());
        }
        return res;
    } else {
        return std::nullopt;
    }
}

static opt<size_t> extr_p(std::string_view s) noexcept {
    if (auto m = ctre::match<R"re(^\s*\.p\s+(\d+)\s*$)re">(s)) {
        const auto nsv      = m.get<1>().to_view();
        size_t n            = ~0;
        const auto conv_res = std::from_chars<size_t>(nsv.cbegin(), nsv.cend(), n, 10);
        if (conv_res.ec != std::errc{}) {
            return ~0;
        }
        return n;
    } else {
        return std::nullopt;
    }
}

static bool extr_e(std::string_view s) noexcept {
    return ctre::match<R"re(^\s*\.e\s*$)re">(s);
}

SOP read_pla_file(const std::string &pla_path) {
    SOP pla;
    const auto pla_str = slurp_file_string(pla_path);
    if (!pla_str.size()) {
        fprintf(stderr, "pla file read error\n");
        std::terminate();
    }
    opt<std::vector<std::string>> ilb;
    opt<std::vector<std::string>> ob;
    opt<size_t> p;
    bool in_header = true;
    for (const auto &line : pla_str | std::ranges::views::split('\n')) {
        const std::string_view lsv{line};
        if (in_header) {
            if (!ilb && (ilb = extr_ilb(lsv))) {
                // .ilb
            } else if (!ob && (ob = extr_ob(lsv))) {
                // .ob
            } else if (!p && (p = extr_p(lsv))) {
                // .p
                in_header = false;
            }
        } else {
            if (const auto imp = extr_implicant(lsv)) {
                pla.add_implicant(std::move(*imp));
            } else if (extr_e(lsv)) {
                // .e
                break;
            }
        }
    }
    // TODO: return labels
    return pla;
}
