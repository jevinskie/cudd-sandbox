#pragma once

#include <string>

#include <cudd.h>

int reorder_dddmp_file(const std::string &in_path, const std::string &out_path);
int import_sop_pla(const std::string &in_path, const std::string &out_path);
