#include <cudd-sandbox/cudd-sandbox.hpp>

#include <cudd.h>
#include <flags.h>
#include <fmt/format.h>

int main(int argc, char **argv) {
    const flags::args args(argc, argv);
    const auto in_path     = args.get<std::string>("in");
    const auto out_path    = args.get<std::string>("out");
    const auto reorder_cmd = args.get<bool>("reorder", false);
    const auto pla_cmd     = args.get<bool>("pla", false);
    if (reorder_cmd) {
        if (!in_path) {
            fmt::print(stderr, "reorder: need --in path\n");
            return 1;
        }
        if (!out_path) {
            fmt::print(stderr, "reorder: need --out path\n");
            return 1;
        }
        fmt::print("reordering {:s} into {:s}\n", *in_path, *out_path);
        const auto reorder_res = reorder_dddmp_file(*in_path, *out_path);
        fmt::print("result: {}\n", reorder_res);
    } else if (pla_cmd) {
        if (!in_path) {
            fmt::print(stderr, "pla: need --in path\n");
            return 1;
        }
        if (!out_path) {
            fmt::print(stderr, "pla: need --out path\n");
            return 1;
        }
        fmt::print("pla reading {:s} into {:s}\n", *in_path, *out_path);
        const auto sop_pla_res = import_sop_pla(*in_path, *out_path);
        fmt::print("import_sop_pla returned: {}\n", sop_pla_res);
    }
    return 0;
}
