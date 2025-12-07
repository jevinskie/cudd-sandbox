#include <cudd-sandbox/cudd-sandbox.hpp>

#include <flags.h>
#include <fmt/format.h>

int main(int argc, char **argv) {
    const flags::args args(argc, argv);
    const auto in_path     = args.get<std::string>("in");
    const auto out_path    = args.get<std::string>("out");
    const auto reorder_cmd = args.get<bool>("reorder", false);
    if (reorder_cmd) {
        if (!in_path) {
            fmt::print(stderr, "need --in path\n");
            return 1;
        }
        if (!out_path) {
            fmt::print(stderr, "need --out path\n");
            return 1;
        }
        fmt::print("reordering {:s} into {:s}\n", *in_path, *out_path);
        const auto ok = reorder_dddmp_file(*in_path, *out_path);
        fmt::print("result: {:s}\n", ok ? "OK" : "BAD");
    }
    return 0;
}
