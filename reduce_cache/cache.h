/**
 * @file cache.h
 * @author phanen
 * @brief header for macro definition and cache declaration
 * @date 2022-05-14
 *
 * @copyright Copyright (c) 2022. phanen
 *
 */

#include <iostream>
#include <cstdint>
#include <fstream>
#include <cassert>
//
#include <deque>
#include <map>
#include <vector>

// cache param
uint8_t addr_wd = 32;                               // main mem address width
uint32_t set_wd = 6;                                // width of set_num
uint32_t assoc_wd = 2;                              // width of assoc_num
uint32_t block_wd = 4;                              // width of block_sz
uint32_t set_num = 1 << set_wd;                     // 2^set_wd, number of sets
uint32_t assoc_num = 1 << assoc_wd;                 // 2^assoc_wd, associativity (line/set)
uint32_t block_sz = 1 << block_wd;                  // 2^block_wd, Unit: B
uint32_t cache_sz = assoc_num * set_num * block_sz; // 2^(block_wd + assoc_wd + set_wd)
uint32_t tag_wd = addr_wd - set_wd - block_wd;

// counter -- for model check
int access_num = 0;
int hit_num = 0;

/**
 * @brief show the configuration of cache
 */
inline void log_cfg() {
    printf("cache size:          %d B\n", cache_sz);
    printf("block size:          %d B\n", block_sz);
    printf("set num:             %d\n", set_num);
    printf("associativity:       %d\n", assoc_num);
    // printf("\n");
}

/**
 * @brief show the info of mem access
 */
inline void log_cnt() {
    printf("access num:                 %d\n", access_num);
    printf("hit num:                    %d\n", hit_num);
    printf("hit rate:                   %lf\n",
           double(hit_num) / double(access_num));
    // printf("\n");
}

void log_() {
    // log_cfg();
    // log_cnt();
    printf("hit rate:                   %lf\n",
           double(hit_num) / double(access_num));
    printf("\n");
}

inline void cache_cfg(uint32_t a, uint32_t b, uint32_t c) {
    set_wd = a;
    assoc_wd = b;
    block_wd = c;
    set_num = 1 << set_wd;
    assoc_num = 1 << assoc_wd;
    block_sz = 1 << block_wd;
    cache_sz = assoc_num * set_num * block_sz;
    tag_wd = addr_wd - set_wd - block_wd;
    assert(set_num >= 1 && assoc_num >= 1 && block_sz >= 1 && cache_sz >= 1);
    assert(tag_wd > 0);
    access_num = hit_num = 0;
}