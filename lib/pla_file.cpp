#include <cudd-sandbox/pla_file.hpp>

#include <cudd-sandbox/utils.hpp>

#include <cinttypes>
#include <exception>
#include <optional>
#include <ranges>

#include <ctre.hpp>
#include <string>
#include <string_view>

std::optional<Implicant> extr_implicant(std::string_view s) noexcept {
    if (auto m = ctre::match<R"re(^\s*([01\-]+)\s+([01~]+)\s*$)re">(s)) {
        return Implicant(m.get<1>().to_view(), m.get<2>().to_view());
    } else {
        return std::nullopt;
    }
}

SOP read_pla_file(const std::string &pla_path) {
    SOP pla;
    const auto plas = slurp_file_string(pla_path);
    if (!plas.size()) {
        fprintf(stderr, "pla file read error\n");
        std::terminate();
    }
    int i = 0;
    for (const auto &line : plas | std::ranges::views::split('\n')) {
        ++i;
        printf("pla[%5d]_i: %.*s\n", i, static_cast<int>(line.size()), line.data());
        if (const auto imp = extr_implicant(std::string_view(line))) {
            // printf("pla[%5d]_o: ibm: 0x%08" PRIx64 " ibp: 0x%08" PRIx64 " obm: 0x%08" PRIx64 " obp: 0x%08" PRIx64
            //         "\n", i, imp->in_bmask(), imp->in_bpat(), imp->out_bmask(), imp->out_bpat());
            printf("pla[%5d]_o: b[m,p]: 0x%08" PRIx64 ", 0x%08" PRIx64 "\n", i, imp->in_bmask(), imp->in_bpat());
        }
    }
    return pla;
}
