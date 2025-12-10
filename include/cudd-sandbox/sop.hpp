#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <string_view>
#include <utility>
#include <vector>

class Implicant {
public:
    Implicant(std::string_view in, std::string_view out) {
        if (!is_valid_in_chars(in) || !is_valid_out_chars(out)) {
            std::terminate();
        }
        _in_bmask  = parse_bmask(in);
        _in_bpat   = parse_bpat(in);
        _out_bmask = parse_bmask(out);
        _out_bpat  = parse_bpat(out);
    }

    size_t in_sz() const {
        return _in_sz;
    }
    size_t out_sz() const {
        return _out_sz;
    }
    std::pair<size_t, size_t> sz() const {
        return std::make_pair(in_sz(), out_sz());
    }
    uint64_t in_bmask() const {
        return _in_bmask;
    }
    uint64_t in_bpat() const {
        return _in_bpat;
    }
    uint64_t out_bmask() const {
        return _out_bmask;
    }
    uint64_t out_bpat() const {
        return _out_bpat;
    }

    static bool is_valid_in_chars(std::string_view ibs) {
        bool good = true;
        if (ibs.size() > 64) {
            fprintf(stderr, "Implicant input length %zu > 64\n", ibs.size());
            good = false;
        }
        for (const auto ic : ibs) {
            if (ic != '0' && ic != '1' && ic != '-') {
                fprintf(stderr, "Implicant input '%s' \"bit\" '%c' is not 0, 1, or -\n", ibs.data(), ic);
                good = false;
            }
        }
        return good;
    }

    static bool is_valid_out_chars(std::string_view obs) {
        bool good = true;
        if (obs.size() > 64) {
            fprintf(stderr, "Implicant output length %zu > 64\n", obs.size());
            good = false;
        }
        for (const auto oc : obs) {
            if (oc != '0' && oc != '1' && oc != '~') {
                fprintf(stderr, "Implicant output '%s' \"bit\" '%c' is not 0, 1, or ~\n", obs.data(), oc);
                good = false;
            }
        }
        return good;
    }

    static uint64_t parse_bmask(std::string_view bs) {
        uint64_t bm = 0;
        for (const auto c : bs) {
            bm <<= 1;
            if (c == '0' || c == '1') {
                bm |= 1;
            }
        }
        return bm;
    }

    static uint64_t parse_bpat(std::string_view bs) {
        uint64_t bp = 0;
        for (const auto c : bs) {
            bp <<= 1;
            if (c == '1') {
                bp |= 1;
            }
        }
        return bp;
    }

private:
    uint64_t _in_bmask;
    uint64_t _in_bpat;
    uint64_t _out_bmask;
    uint64_t _out_bpat;
    uint8_t _in_sz;
    uint8_t _out_sz;
};

class SOP {
public:
    SOP() {}
    void add_implicant(const Implicant &imp) {
        if (imp.in_sz() == 0 || imp.out_sz() == 0) {
            fprintf(stderr, "implicants must be positively sized. in sz: %zu out sz: %zu\n", imp.in_sz(), imp.out_sz());
            std::terminate();
        }
        _imps.push_back(imp);
    }
    const std::vector<Implicant> &implicants() const {
        return _imps;
    }

    size_t in_sz() const {
        return _in_sz;
    }
    size_t out_sz() const {
        return _out_sz;
    }
    std::pair<size_t, size_t> sz() const {
        return std::make_pair(in_sz(), out_sz());
    }

private:
    uint64_t _in_sz  = 0;
    uint64_t _out_sz = 0;
    std::vector<Implicant> _imps;
};
