#include <cudd-sandbox/reorder.hpp>

#include <cudd-sandbox/pla_file.hpp>

#undef NDEBUG
#include <cassert>

#include <exception>

#include <cudd.h>
#include <dddmp.h>
#include <fmt/format.h>

int reorder_dddmp_file(const std::string &in_path, const std::string &out_path) {
    std::string ip = in_path;
    std::string op = out_path;
    DdManager *mgr = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    Cudd_PrintInfo(mgr, stderr);
    DdNode *res =
        Dddmp_cuddBddLoad(mgr, DDDMP_VAR_MATCHIDS, nullptr, nullptr, nullptr, DDDMP_MODE_TEXT, ip.data(), nullptr);
    if (!res) {
        return -1;
    }
    assert(res);
    fprintf(stderr, "chk2\n");
    Cudd_DebugCheck(mgr);
    fprintf(stderr, "chk2d\n");

    fprintf(stderr, "\n\n\n");
    Cudd_PrintInfo(mgr, stderr);
    const char *varnames[] = {"a", "b", "c", "d"};
    const auto store_res   = Dddmp_cuddBddStore(mgr, const_cast<char *>("opt"), res, const_cast<char **>(varnames),
                                                nullptr, DDDMP_MODE_TEXT, DDDMP_VARDEFAULT, op.data(), nullptr);
    Cudd_Quit(mgr);
    return store_res;
}

int import_sop_pla(const std::string &in_path, const std::string &out_path) {
    std::string ip = in_path;
    std::string op = out_path;
    // Cudd_PrintInfo(mgr, stderr);
    const auto sop = read_pla_file(ip);
    std::vector<std::string> varnames;
    std::vector<const char *> varnames_cstr;
    std::vector<DdNode *> vars;
    std::vector<DdNode *> imps;
    const auto num_in = sop.in_sz();
    fmt::print("sop # in: {} # out: {} # terms: {}\n", sop.in_sz(), sop.out_sz(), sop.implicants().size());
    DdManager *mgr = Cudd_Init(num_in, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    for (size_t i = 0; i < num_in; ++i) {
        varnames.push_back(fmt::format("i{}", i));
        const auto v = Cudd_bddIthVar(mgr, i);
        if (!v) {
            std::terminate();
        }
        vars.push_back(v);
    }
    for (const auto &vn : varnames) {
        varnames_cstr.push_back(vn.data());
    }
    Cudd_DebugCheck(mgr);

    for (const auto &imp : sop.implicants()) {
        const auto bm    = imp.in_bmask();
        const auto bp    = imp.in_bpat();
        DdNode *imp_node = Cudd_ReadOne(mgr);
        Cudd_Ref(imp_node);
        for (size_t i = 0; i < num_in; ++i) {
            const auto ibm = !!((bm >> i) & 1);
            if (!ibm) {
                continue;
            }
            const auto ibp = !!((bp >> i) & 1);
            DdNode *zero   = Cudd_ReadLogicZero(mgr);
            Cudd_Ref(zero);
            DdNode *tmp =
                Cudd_bddIte(mgr, ibp ? Cudd_bddIthVar(mgr, i) : Cudd_Not(Cudd_bddIthVar(mgr, i)), imp_node, zero);
            Cudd_Ref(tmp);
            Cudd_RecursiveDeref(mgr, imp_node);
            imp_node = tmp;
        }
        imps.push_back(imp_node);
    }

    DdNode *out_node = Cudd_ReadLogicZero(mgr);
    Cudd_Ref(out_node);
    for (auto *imp : imps) {
        DdNode *one = Cudd_ReadOne(mgr);
        Cudd_Ref(one);
        DdNode *tmp = Cudd_bddIte(mgr, imp, one, out_node);
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(mgr, out_node);
        out_node = tmp;
    }

    const auto reduce_res = Cudd_ReduceHeap(mgr, CUDD_REORDER_SIFT, 0);
    fmt::print("reduce_res: {}\n", reduce_res);
    const auto reduce_res2 = Cudd_ReduceHeap(mgr, CUDD_REORDER_EXACT, 0);
    fmt::print("reduce_res2: {}\n", reduce_res2);

    Cudd_PrintInfo(mgr, stderr);
    const auto store_res =
        Dddmp_cuddBddStore(mgr, const_cast<char *>("opt"), out_node, const_cast<char **>(varnames_cstr.data()), nullptr,
                           DDDMP_MODE_TEXT, DDDMP_VARDEFAULT, op.data(), nullptr);
    Cudd_PrintDebug(mgr, out_node, num_in, 1);
    Cudd_Quit(mgr);
    return store_res;
}
