#pragma once

#include <cudd-sandbox/sop.hpp>

#include <string_view>

SOP read_pla_file(std::string_view pla_path);
