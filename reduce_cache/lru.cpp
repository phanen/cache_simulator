/**
 * @file lru.cpp
 * @author phanen
 * @brief
 *
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include "cache.h"
#include <list>
#include <unordered_map>
using namespace std;

typedef list<uint32_t> lst_t;
typedef unordered_map<uint32_t, list<uint32_t>::iterator> tab_t;
lst_t *cache;
// tag to iterator
tab_t *id_tabs;

// current val
uint32_t cur_addr;
uint32_t cur_tag;
uint32_t cur_set_idx;

static inline void addr_parse() {
    cur_set_idx = (cur_addr >> block_wd) & (set_num - 1);
    cur_tag = cur_addr >> (block_wd + set_wd);
}
int cnt;
static inline void access() {
    bool is_hit = (id_tabs[cur_set_idx].find(cur_tag) != id_tabs[cur_set_idx].end());

    if (is_hit) {
        ++hit_num;
        // move the accessed one to front
        list<uint32_t>::iterator it0 = id_tabs[cur_set_idx][cur_tag];
        cache[cur_set_idx].push_front(*it0);
        cache[cur_set_idx].erase(it0);
        id_tabs[cur_set_idx][cur_tag] = cache[cur_set_idx].begin();

    } else {
        // cold miss
        if (cache[cur_set_idx].size() < assoc_num) {
            cache[cur_set_idx].push_front(cur_tag);
            id_tabs[cur_set_idx].insert(make_pair(cur_tag, cache[cur_set_idx].begin()));
        } else { // lru replace
            // pop out the leastly used one
            uint32_t last_tag = cache[cur_set_idx].back();
            id_tabs[cur_set_idx].erase(last_tag);
            cache[cur_set_idx].pop_back();
            // push in the new one
            cache[cur_set_idx].push_front(cur_tag);
            id_tabs[cur_set_idx].insert(make_pair(cur_tag, cache[cur_set_idx].begin()));
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

void cache_init() {
    cache = new lst_t[set_num];
    id_tabs = new tab_t[set_num];
}

void cache_destroy() {
    delete[] cache;
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

        trace_access(trace_in);
        cache_destroy();
        log_();
        trace_in.close();
    }
    cfg_in.close();
}
int main() {
    t();
}