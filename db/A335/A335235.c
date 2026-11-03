/*
 * A335235  Numbers k such that the k-th composition in standard order
 *          (A066099) is pairwise coprime, where a singleton is always
 *          considered coprime.
 * https://oeis.org/A335235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate compositions in A066099 order and collect the
   indices whose composition consists of pairwise coprime parts. */
/* External utilities are used for reallocation and gcd. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

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

seq_t a(idx_t n)
{
    /* Static cache for found indices. */
    static seq_t  *cache  = NULL;     /* array of valid k's */
    static idx_t  cache_cap = 0;      /* allocated capacity */
    static idx_t  cache_len = 0;      /* number of stored values   */
    static idx_t  last_k    = 0;      /* last k processed          */

    /* Expand cache until it contains at least n elements. */
    while (cache_len < n) {
        ++last_k;                    /* next k to test */
        uint64_t k = (uint64_t)last_k;  /* copy for bit manipulation */
        uint64_t pos = 0;              /* previous set‑bit position */
        uint64_t bit_pos = 1;          /* current bit index (1‑based) */

        /* At most 64 part sizes for k < 2^64. */
        uint64_t parts[64];
        uint64_t num_parts = 0;

        /* Determine the composition corresponding to k. */
        while (k) {
            if (k & 1ULL) {
                uint64_t diff = bit_pos - pos;
                parts[num_parts++] = diff;
                pos = bit_pos;
            }
            k >>= 1;
            ++bit_pos;
        }

        /* Test pairwise coprimality. */
        bool ok = true;
        for (uint64_t i = 0; i < num_parts && ok; ++i) {
            for (uint64_t j = i + 1; j < num_parts; ++j) {
                if (gcd((seq_t)parts[i], (seq_t)parts[j]) != (seq_t)1) {
                    ok = false;
                    break;
                }
            }
        }

        /* If valid, store the index, enlarging the cache if necessary. */
        if (ok) {
            if (cache_len >= cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();              /* out of memory */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
