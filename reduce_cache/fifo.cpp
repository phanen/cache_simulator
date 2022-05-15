/**
 * @file fifo.cpp
 * @author phanen
 * @brief
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include "cache.h"
#include <cstdlib>
#include <deque>
// #include <fifo>

using namespace std;

typedef deque<uint32_t> dq_t;
dq_t *cache_dq;

// current val
uint32_t cur_addr;
uint32_t cur_tag;
uint32_t cur_set_idx;

inline bool imple_switch();

static inline void addr_parse() {
    cur_set_idx = (cur_addr >> block_wd) & (set_num - 1);
    cur_tag = cur_addr >> (block_wd + set_wd);
}

static inline void access_dq() {
    bool is_hit = false;

    for (auto &each_line : cache_dq[cur_set_idx]) {
        if (each_line == cur_tag) {
            ++hit_num;
            is_hit = true;
            break;
        }
    }
    if (!is_hit) {
        // cold miss
        if (cache_dq[cur_set_idx].size() < assoc_num) {
            cache_dq[cur_set_idx].push_back(cur_tag);
        } else { // fifo replace
            cache_dq[cur_set_idx].pop_front();
            cache_dq[cur_set_idx].push_back(cur_tag);
        }
    }
}

static inline void trace_access_dq(std::ifstream &trace_file) {
    char dump;
    trace_file >> std::hex;
    std::cout << std::hex;
    while (trace_file >> dump >> cur_addr) {
        ++access_num;
        addr_parse();
        access_dq();
    }
}

static inline void trace_access_s(std::ifstream &trace_file) {
    char dump;
    trace_file >> std::hex;
    std::cout << std::hex;
    while (trace_file >> dump >> cur_addr) {
        ++access_num;
        addr_parse();
        // access_s();
    }
}

void cache_init() {
    if (!imple_switch()) {
        cache_dq = new dq_t[set_num];
    } else {
    }
}

void cache_destroy() {
    if (!imple_switch()) {
        delete[] cache_dq;
    } else {
    }
}

inline bool imple_switch() {
    // return assoc_wd > 8;
    // return true;
    return false;
}

void t() {
    uint32_t a, b, c;
    ifstream cfg_in, trace_in;

    cfg_in.open(".\\test\\cfg1.txt", ios::in);
    while (cfg_in >> a >> b >> c) {
        cache_cfg(a, b, c);
        cache_init();

        // trace_in.open("..\\benchmark\\trace1.txt", ios::in);
        trace_in.open("..\\benchmark\\aa.txt", ios::in);
        // trace_in.open("..\\benchmark\\gcc.trace", ios::in);

        if (!imple_switch()) {
            trace_access_dq(trace_in);
        } else {
            // trace_access_s(trace_in);
        }

        log_();
        cache_destroy();

        trace_in.close();
    }
    cfg_in.close();
}

int main() {
    t();
}