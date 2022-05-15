/**
 * @file rand.cpp
 * @author phanen
 * @brief
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include "cache.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

// small assoc
typedef vector<uint32_t> vec_t;
vec_t *cache_v;
// set assoc
typedef set<uint32_t> set_t;
set_t *cache_s;

// current val
uint32_t cur_addr;
uint32_t cur_tag;
uint32_t cur_set_idx;

inline bool imple_switch();

static inline void addr_parse() {
    cur_set_idx = (cur_addr >> block_wd) & (set_num - 1);
    cur_tag = cur_addr >> (block_wd + set_wd);
}

static inline void access() {
    bool is_hit = false;

    for (auto &each_line : cache_v[cur_set_idx]) {
        if (each_line == cur_tag) {
            ++hit_num;
            is_hit = true;
            break;
        }
    }
    if (!is_hit) {
        // cold miss
        if (cache_v[cur_set_idx].size() < assoc_num) {
            cache_v[cur_set_idx].push_back(cur_tag);
        } else { // rand replace
            cache_v[cur_set_idx][rand() % assoc_num] = cur_tag;
        }
    }
}

static inline void access_s() {
    bool is_hit = (cache_s[cur_set_idx].find(cur_tag) != cache_s[cur_set_idx].end());

    if (is_hit) {
        ++hit_num;
    } else {
        // cold miss
        if (cache_s[cur_set_idx].size() < assoc_num) {
            cache_s[cur_set_idx].insert(cur_tag);
        } else { // rand replace
            auto it(cache_s[cur_set_idx].begin());
            advance(it, rand() % assoc_num);
            cache_s[cur_set_idx].erase(it);
            cache_s[cur_set_idx].insert(cur_tag);
        }
    }
}

static inline void trace_access(std::ifstream &trace_file) {
    char dump;
    trace_file >> std::hex;
    std::cout << std::hex;
    while (trace_file >> dump >> cur_addr) {
        ++access_num;
        addr_parse();
        access();
    }
}

static inline void trace_access_s(std::ifstream &trace_file) {
    char dump;
    trace_file >> std::hex;
    std::cout << std::hex;
    while (trace_file >> dump >> cur_addr) {
        ++access_num;
        addr_parse();
        access_s();
    }
}

void cache_init() {
    srand(time(nullptr));
    if (!imple_switch()) {
        cache_v = new vec_t[set_num];
    } else {
        cache_s = new set_t[set_num];
    }
}

void cache_destroy() {
    if (!imple_switch()) {
        delete[] cache_v;
    } else {
        delete[] cache_s;
    }
}

inline bool imple_switch() {
    // return assoc_wd > 8;
    // return true;
    return false;
}

void test() {
    uint32_t a, b, c;
    ifstream cfg_in, trace_in;

    cfg_in.open(".\\test\\cfg.txt", ios::in);
    while (cfg_in >> a >> b >> c) {
        cache_cfg(a, b, c);
        cache_init();

        trace_in.open(".\\test\\trace.txt", ios::in);

        if (!imple_switch()) {
            trace_access(trace_in);
        } else {
            trace_access_s(trace_in);
        }

        log_();
        cache_destroy();
        trace_in.close();
    }
    cfg_in.close();
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
        // trace_in.open("..\\benchmark\\gzip.trace", ios::in);
        // trace_in.open("..\\benchmark\\jiangxin.trace", ios::in);
        // trace_in.open("..\\benchmark\\swim.trace", ios::in);
        // trace_in.open("..\\benchmark\\trace1.trace", ios::in);
        // trace_in.open("..\\benchmark\\twolf.trace", ios::in);

        if (!imple_switch()) {
            trace_access(trace_in);
        } else {
            trace_access_s(trace_in);
        }

        log_();

        trace_in.close();
    }
    cfg_in.close();
}
int main() {
    // test();
    t();
}