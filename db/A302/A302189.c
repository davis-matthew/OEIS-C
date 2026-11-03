/*
 * A302189  Hurwitz inverse of squares [1,4,9,16,...].
 * https://oeis.org/A302189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 370 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (-1)^n - 3*n*a(n-1) - n*(n-1)*a(n-2)   (n ≥ 2), 
 * a(0) = 1, a(1) = -4
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;   /* memoisation array */
static size_t cap   = 0;      /* current allocated capacity */
static idx_t  last  = (idx_t)-1;   /* last computed index */

/* Return the n-th term of sequence A302189 */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold index n */
    ensure_cache(n, &cache, &cap);

    /* Initialise on first call */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        last = 0;
    }

    /* Compute terms up to n (any order of calls handled by memoisation) */
    while (last < n) {
        idx_t k = last + 1;          /* next index */

        /* (-1)^k */
        seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;

        /* 3*k*a(k-1)  (k ≥ 1) */
        seq_t term2 = (seq_t)3 * (seq_t)k * cache[k - 1];

        /* k*(k-1)*a(k-2)  (zero if k < 2) */
        seq_t term3 = 0;
        if (k >= 2) {
            term3 = (seq_t)k * (seq_t)(k - 1) * cache[k - 2];
        }

        /* a(k) = sign - term2 - term3 */
        cache[k] = sign - term2 - term3;
        last = k;
    }

    return cache[n];
}
