/*
 * A316485  Squares whose arithmetic mean of digits is 5 (i.e., the sum of
 *          digits is 5 times the number of digits).
 * https://oeis.org/A316485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Iteratively generate squares whose digit sum equals 5 times the
 *            number of digits
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 2803, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/digit_sum.h"
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, keep to satisfy compiler */

#ifdef __cplusplus
    static std::vector<seq_t> cache_vec;
    static idx_t next_m = 1;
    while (cache_vec.size() < n) {
        idx_t m = next_m++;
        seq_t sq;
#ifdef __SIZEOF_INT128__
        __uint128_t tmp = (__uint128_t)m * m;
        sq = (seq_t)tmp;
#else
        sq = (seq_t)m * m;
#endif
        int d = digit_len(sq);
        seq_t sum = digit_sum(sq);
        seq_t target = (seq_t)5 * (seq_t)d;
        if (sum == target) {
            cache_vec.push_back(sq);
        }
    }
    return cache_vec[n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_m = 1;
    while (cache_len < n) {
        idx_t m = next_m++;
        seq_t sq;
#ifdef __SIZEOF_INT128__
        __uint128_t tmp = (__uint128_t)m * m;
        sq = (seq_t)tmp;
#else
        sq = (seq_t)m * m;
#endif
        int d = digit_len(sq);
        seq_t sum = digit_sum(sq);
        seq_t target = (seq_t)5 * (seq_t)d;
        if (sum == target) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!newp) abort();
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
    }
    return cache[n - 1];
#endif
}
