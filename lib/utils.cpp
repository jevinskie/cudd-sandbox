#include <cudd-sandbox/utils.hpp>

#include <cstdlib>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

std::string slurp_file_string(const std::string &path) {
    const auto fd = ::open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        std::abort();
    }
    struct stat st;
    if (::fstat(fd, &st)) {
        std::abort();
    }
    const auto sz = static_cast<size_t>(st.st_size);
    if (!sz) {
        ::close(fd);
        std::abort();
    }
    auto res = std::string(sz, '\0');
    if (static_cast<ssize_t>(sz) != ::read(fd, res.data(), res.size())) {
        std::abort();
    }
    if (::close(fd)) {
        std::abort();
    }
    return res;
}
