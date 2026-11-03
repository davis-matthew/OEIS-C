/*
 * A335524  Numbers k such that the k-th composition in standard order
 *          (A066099) avoids the pattern (2,2,1).
 * https://oeis.org/A335524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335524:
   a(n) = n-th integer k such that the k-th composition (in standard order)
   avoids the pattern (2,2,1). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Test whether the k-th composition avoids the pattern (2,2,1). */
static bool avoids_pattern(idx_t k)
{
    /* Positions of 1-bits in k, least‑significant bit has position 1. */
    idx_t pos[64];
    size_t pos_len = 0;
    idx_t bitpos = 1;
    idx_t tmp = k;
    while (tmp) {
        if (tmp & 1ULL) {
            pos[pos_len++] = bitpos;
        }
        tmp >>= 1;
        ++bitpos;
    }

    /* Build the parts list: first difference with 0, then reverse. */
    idx_t parts[64];
    if (pos_len == 0) {
        /* Empty composition – trivially avoids the pattern. */
        return true;
    }
    parts[0] = pos[0];          /* first part = pos[0] - 0 */
    for (size_t i = 1; i < pos_len; ++i) {
        parts[i] = pos[i] - pos[i - 1];
    }
    /* Reverse the array to get the natural order of the composition. */
    for (size_t i = 0; i < pos_len / 2; ++i) {
        idx_t t = parts[i];
        parts[i] = parts[pos_len - 1 - i];
        parts[pos_len - 1 - i] = t;
    }

    /* Scan for a subsequence (2,2,1).  Track part values that have
       appeared at least twice and keep the largest such value. */
    int seen[65] = {0};
    idx_t max_double = 0;
    bool double_seen = false;

    for (size_t i = 0; i < pos_len; ++i) {
        idx_t v = parts[i];
        if (double_seen && v < max_double) {
            return false;          /* pattern found */
        }
        if (seen[v] == 1) {        /* second occurrence -> double */
            double_seen = true;
            if (v > max_double) {
                max_double = v;
            }
        }
        seen[v] = seen[v] + 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* safety for indices < 1 */

    /* Static cache of found k-values. */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;
    static idx_t  next_k    = 0;   /* k to test next */

    while (cache_len < n) {
        if (avoids_pattern(next_k)) {
            /* Allocate or grow the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp;
                if (cache_cap == 0) {
                    tmp = seq_alloc(new_cap);
                    if (!tmp) abort();
                } else {
                    tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
