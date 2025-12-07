#include <cudd-sandbox/reorder.hpp>

#include <dddmp.h>

int reorder_dddmp_file(DdManager *mgr, const std::string &in_path, const std::string &out_path) {
    std::string ip = in_path;
    std::string op = out_path;
    Cudd_PrintInfo(mgr, stderr);
    DdNode *in_node =
        Dddmp_cuddBddLoad(mgr, DDDMP_VAR_MATCHIDS, nullptr, nullptr, nullptr, DDDMP_MODE_DEFAULT, ip.data(), nullptr);
    if (!in_node) {
        return -1;
    }
    fprintf(stderr, "\n\n\n");
    Cudd_PrintInfo(mgr, stderr);
    char dumb_const_qual_workaround[] = "opt";
    const auto store_res              = Dddmp_cuddBddStore(mgr, dumb_const_qual_workaround, in_node, nullptr, nullptr,
                                                           DDDMP_MODE_TEXT, DDDMP_VARDEFAULT, op.data(), nullptr);
    return store_res;
}
