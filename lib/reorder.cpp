#include <cudd-sandbox/reorder.hpp>

#undef NDEBUG
#include <cassert>

#include <cudd.h>
#include <dddmp.h>

int reorder_dddmp_file(DdManager *mgr, const std::string &in_path, const std::string &out_path) {
    std::string ip = in_path;
    std::string op = out_path;
    Cudd_PrintInfo(mgr, stderr);
    // DdNode *in_node =
    //     Dddmp_cuddBddLoad(mgr, DDDMP_VAR_MATCHIDS, nullptr, nullptr, nullptr, DDDMP_MODE_DEFAULT, ip.data(),
    //     nullptr);
    // if (!in_node) {
    //     return -1;
    // }
    const auto a = Cudd_bddNewVar(mgr);
    Cudd_Ref(a);
    const auto b = Cudd_bddNewVar(mgr);
    Cudd_Ref(b);
    const auto c = Cudd_bddNewVar(mgr);
    Cudd_Ref(c);
    const auto d = Cudd_bddNewVar(mgr);
    Cudd_Ref(d);
    assert(a);
    assert(b);
    assert(c);
    assert(d);
    fprintf(stderr, "chk0\n");
    Cudd_DebugCheck(mgr);
    fprintf(stderr, "chk0d\n");
    const auto mt0 = Cudd_bddAnd(mgr, a, b);
    assert(mt0);
    Cudd_Ref(mt0);
    fprintf(stderr, "chk1.a\n");
    Cudd_DebugCheck(mgr);
    fprintf(stderr, "chk1.ad\n");
    const auto mt1 = Cudd_bddAnd(mgr, a, c);
    assert(mt1);
    Cudd_Ref(mt1);
    const auto mt2 = Cudd_bddAnd(mgr, a, d);
    assert(mt2);
    Cudd_Ref(mt2);
    const auto mt3_0 = Cudd_bddAnd(mgr, a, c);
    assert(mt3_0);
    Cudd_Ref(mt3_0);
    const auto mt3_1 = Cudd_bddAnd(mgr, mt3_0, d);
    assert(mt3_1);
    Cudd_Ref(mt3_1);
    const auto mt3 = mt3_1;
    assert(mt3);
    // Cudd_Ref(mt0); Cudd_Ref(mt1); Cudd_Ref(mt2); Cudd_Ref(mt3_0); Cudd_Ref(mt3_1);
    fprintf(stderr, "chk1\n");
    Cudd_DebugCheck(mgr);
    fprintf(stderr, "chk1d\n");
    const auto res_0 = Cudd_bddOr(mgr, mt0, mt1);
    assert(res_0);
    Cudd_Ref(res_0);
    const auto res_1 = Cudd_bddOr(mgr, res_0, mt2);
    assert(res_1);
    Cudd_Ref(res_1);
    const auto res_2 = Cudd_bddOr(mgr, res_1, mt3);
    assert(res_2);
    Cudd_Ref(res_2);
    const auto res = res_2;

    // const auto res_0 = Cudd_bddXor(mgr, a, b);
    // Cudd_Ref(res_0);
    // const auto res_1 = Cudd_bddAnd(mgr, a, b);
    // Cudd_Ref(res_1);
    // const auto res_2 = Cudd_bddOr(mgr, res_0, res_1);
    // Cudd_Ref(res_2);
    // const auto res_3 = Cudd_bddOr(mgr, res_2, c);
    // Cudd_Ref(res_3);
    // const auto res = res_3;

    assert(res);
    fprintf(stderr, "chk2\n");
    Cudd_DebugCheck(mgr);
    fprintf(stderr, "chk2d\n");

    fprintf(stderr, "\n\n\n");
    Cudd_PrintInfo(mgr, stderr);
    const char *varnames[] = {"a", "b", "c", "d"};
    const auto store_res   = Dddmp_cuddBddStore(mgr, const_cast<char *>("opt"), res, const_cast<char **>(varnames),
                                                nullptr, DDDMP_MODE_TEXT, DDDMP_VARDEFAULT, op.data(), nullptr);
    return store_res;
    // Dddmp_cuddBddDisplayBinary(ip.data(), op.data());
    // std::string tmp = op + ".t2b";
    // std::string out = op + ".b2t";
    // const auto t2br = Dddmp_Text2Bin(ip.data(), tmp.data());
    // if (t2br != 1) {
    //     fprintf(stderr, "t2b failed %d\n", t2br);
    //     return t2br;
    // }
    // const auto b2tr = Dddmp_Bin2Text(tmp.data(), out.data());
    // if (b2tr != 1) {
    //     fprintf(stderr, "b2t failed %d\n", b2tr);
    // }
    // return b2tr;
    // const auto dbgr = Dddmp_cuddBddDisplayBinary(ip.data(), op.data());
    // fprintf(stderr, "dbgr: %d\n", dbgr);
    // return dbgr;
}
