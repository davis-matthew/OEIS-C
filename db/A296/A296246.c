/*
 * A296246  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n)^2, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296246
   a(n) = a(n-1) + a(n-2) + b(n)^2
   a(0)=1, a(1)=3
   b(0)=2, b(1)=4, b(2)=5
   a() and b() are increasing complementary sequences. */

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifndef __cplusplus
    /* ---------- C implementation ---------- */
    static seq_t *a_cache = NULL;
    static idx_t  a_cap   = 0;
    static idx_t  a_len   = 0;
    static seq_t *b_cache = NULL;
    static idx_t  b_cap   = 0;
    static idx_t  b_len   = 0;
    static seq_t  cur_t   = 1;   /* next integer to test */
    static idx_t  a_idx_for_scan = 0;

    /* initialize a_cache with a(0) and a(1) */
    if (a_len == 0) {
        size_t new_cap = 2;
        a_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!a_cache) return (seq_t)0;
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        a_cap   = new_cap;
        a_len   = 2;
    }

    /* fill b_cache up to index n */
    while (b_len <= n) {
        seq_t b_val;
        for (;;) {
            if (a_idx_for_scan < a_len && a_cache[a_idx_for_scan] == cur_t) {
                ++cur_t;
                ++a_idx_for_scan;
                continue;
            } else {
                b_val = cur_t;
                ++cur_t;
                break;
            }
        }

        if (b_cap <= b_len) {
            size_t new_cap = b_cap ? b_cap : 2;
            while (new_cap <= b_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            b_cache = tmp;
            b_cap   = new_cap;
        }
        b_cache[b_len] = b_val;
        ++b_len;
    }

    /* compute a_cache up to index n */
    while (a_len <= n) {
        seq_t b_val = b_cache[a_len]; /* b(a_len) */
        seq_t new_a = a_cache[a_len-1] + a_cache[a_len-2] + b_val * b_val;
        if (a_cap <= a_len) {
            size_t new_cap = a_cap ? a_cap : 2;
            while (new_cap <= a_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            a_cache = tmp;
            a_cap   = new_cap;
        }
        a_cache[a_len] = new_a;
        ++a_len;
    }

    return a_cache[n];
#else
    /* ---------- C++ implementation ---------- */
    static std::vector<seq_t> a_cache;
    static std::vector<seq_t> b_cache;
    static idx_t  a_idx_for_scan = 0;
    static seq_t  cur_t   = 1;

    if (a_cache.empty()) {
        a_cache.reserve(2);
        a_cache.push_back((seq_t)1);
        a_cache.push_back((seq_t)3);
    }

    /* ensure b_cache up to index n */
    while ((idx_t)b_cache.size() <= n) {
        seq_t b_val;
        for (;;) {
            if (a_idx_for_scan < a_cache.size() && a_cache[a_idx_for_scan] == cur_t) {
                ++cur_t;
                ++a_idx_for_scan;
                continue;
            } else {
                b_val = cur_t;
                ++cur_t;
                break;
            }
        }
        b_cache.push_back(b_val);
    }

    /* ensure a_cache up to index n */
    while ((idx_t)a_cache.size() <= n) {
        seq_t b_val = b_cache[a_cache.size()]; /* b(a_cache.size()) */
        seq_t new_a = a_cache.back() + a_cache[a_cache.size()-2] + b_val * b_val;
        a_cache.push_back(new_a);
    }

    return a_cache[n];
#endif
}
