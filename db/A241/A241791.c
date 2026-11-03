/*
 * A241791  Triangular numbers which have one or more occurrences of exactly
 *          eight different digits.
 * https://oeis.org/A241791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th triangular number that contains exactly eight
 *            distinct decimal digits
 * Range:     u8 none, u16 none, u32 n <= 2000, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 1896 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: does a number contain exactly eight distinct decimal digits? */
static bool has_8_distinct(seq_t x) {
    unsigned char seen[10] = {0};
    unsigned int distinct = 0;
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (!seen[d]) {
            seen[d] = 1;
            ++distinct;
            if (distinct > 8) return false;
        }
        x = x / 10;
    }
    return distinct == 8;
}

/* The main sequence function. */
seq_t a(idx_t n) {
    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_tri_idx = 1;        /* Index m of triangular numbers T(m). */

    /* Grow cache until we have the n-th term. */
    while ((idx_t)cache_len < n) {
        seq_t tri = ((seq_t)next_tri_idx * ((seq_t)next_tri_idx + 1)) / 2;
        if (has_8_distinct(tri)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) abort();   /* Out of memory. */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = tri;
        }
        ++next_tri_idx;
    }

    return cache[n - 1];
}
