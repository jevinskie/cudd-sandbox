#pragma once

#include <string_view>

[[gnu::visibility("default")]]
bool reorder_dddmp_file(const std::string_view &in, const std::string_view &out_path);
