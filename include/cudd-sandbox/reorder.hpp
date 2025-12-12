#pragma once

#include <string>

#include <cudd.h>

int reorder_dddmp_file(DdManager *mgr, const std::string &in_path, const std::string &out_path);
int import_sop_pla(DdManager *mgr, const std::string &in_path, const std::string &out_path);
