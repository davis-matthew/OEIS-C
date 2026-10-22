/*
 * A335488  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (1,1).
 * https://oeis.org/A335488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335488: numbers k such that the k‑th composition in graded reverse‑lexicographic order has a repeated part.
   Implementation: generate all compositions in order, test each for duplicate parts, and keep the rank k of
   those that satisfy the pattern (1,1).  No closed formula is known. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;
    static int initialized = 0;

    if (!initialized) {
        cache_cap = 64; /* initial capacity, will grow as needed */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_k = 1;
        initialized = 1;
    }

    /* Extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        idx_t k = next_k;
        /* Find n_comp: ceil(log2(k+1)) = floor_log2(k) + 1 */
        idx_t n_comp = floor_log2_uint64(k) + 1;
        idx_t r = k - ((idx_t)1 << (n_comp - 1)); /* r in [0, 2^{n_comp-1}-1] */

        /* Build the composition of n_comp from r */
        int parts[64];
        int len = 0;
        int cur = 1;
        for (idx_t i = 0; i < (idx_t)(n_comp - 1); ++i) {
            if ((r >> i) & 1ULL) {
                parts[len++] = cur;
                cur = 1;
            } else {
                cur++;
            }
        }
        parts[len++] = cur; /* last part */

        /* Check for any duplicate part */
        int has_dup = 0;
        for (int i = 0; i < len; ++i) {
            for (int j = 0; j < i; ++j) {
                if (parts[i] == parts[j]) {
                    has_dup = 1;
                    break;
                }
            }
            if (has_dup) break;
        }

        if (has_dup) {
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
