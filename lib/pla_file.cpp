#include <cudd-sandbox/pla_file.hpp>

#include <cudd-sandbox/utils.hpp>

#include <cinttypes>
#include <exception>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include <ctre.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

std::optional<Implicant> extr_implicant(std::string_view s) noexcept {
    if (auto m = ctre::match<R"re(^\s*([01\-]+)\s+([01~]+)\s*$)re">(s)) {
        return Implicant(m.get<1>().to_view(), m.get<2>().to_view());
    } else {
        return std::nullopt;
    }
}

std::optional<std::vector<std::string>> extr_ilb(std::string_view s) noexcept {
    if (auto m = ctre::match<R"re(^\s*\.ilb\s+((?<a>\S+)\s+)*(?<b>\S+)\s*$)re">(s)) {
        const auto ilb_joined = m.get<2>().to_view();
        fmt::print("0: '{}'\n1: '{}'\n2: '{}'\n3: '{}'\n", m.get<0>().to_view(), m.get<1>().to_view(),
                   m.get<2>().to_view(), m.get<3>().to_view());
        fmt::print("a: '{}'\nb: '{}'\n", m.get<"a">().to_view(), m.get<"b">().to_view());
        fmt::print("ilb_joined: {}\n", ilb_joined);
        std::vector<std::string> res;
        for (const auto ilbl : ctre::tokenize<R"re((\S+\s+|\S+)+)re">(m.get<1>())) {
            fmt::print("ibl: {}\n", ilbl.get<0>().to_view());
            res.push_back(ilbl.get<1>().to_string());
        }
        return res;
    } else {
        return std::nullopt;
    }
}

SOP read_pla_file(const std::string &pla_path) {
    SOP pla;
    const auto pla_str = slurp_file_string(pla_path);
    if (!pla_str.size()) {
        fprintf(stderr, "pla file read error\n");
        std::terminate();
    }
    bool in_header = true;
    int i          = 0;
    for (const auto &line : pla_str | std::ranges::views::split('\n')) {
        ++i;
        printf("pla[%5d]_i: %.*s\n", i, static_cast<int>(line.size()), line.data());
        if (in_header) {
            if (const auto ilb = extr_ilb(std::string_view(line))) {
                fmt::print("pla[{:5d}]_ilb: {}\n", i, fmt::join(*ilb, " "));
            }
        } else {
            if (const auto imp = extr_implicant(std::string_view(line))) {
                // printf("pla[%5d]_o: ibm: 0x%08" PRIx64 " ibp: 0x%08" PRIx64 " obm: 0x%08" PRIx64 " obp: 0x%08" PRIx64
                //         "\n", i, imp->in_bmask(), imp->in_bpat(), imp->out_bmask(), imp->out_bpat());
                printf("pla[%5d]_o: b[m,p]: 0x%08" PRIx64 ", 0x%08" PRIx64 "\n", i, imp->in_bmask(), imp->in_bpat());
            }
        }
    }
    return pla;
}
