/*
 * A307273  Partial sums of A307203.
 * https://oeis.org/A307273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 a(2) = 11 a(3) = 21 a(4) = 35 a(5) = 56 a(6) =
 *            82 a(7) = 114 a(8) = 152 a(9) = 198 a(10) = 249 a(11) = 305
 *            a(12) = 366 a(13) = 437 a(14) = 510 a(15) = 588 a(16) = 672 a(n)
 *            = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-5) + 2*a(n-6) -
 *            2*a(n-7) + a(n-8) for n > 16
 * Range:     u8 n <= 10, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memcpy */

static seq_t *cache = NULL;          /* cache of computed values */
static size_t cache_cap = 0;          /* allocated capacity of cache */
static size_t cache_filled = 0;       /* number of valid entries already computed */

/* 17 initial values, indices 0 … 16 */
static const seq_t base_vals[17] = {
    1, 4, 11, 21, 35,
    56, 82, 114, 152, 198,
    249, 305, 366, 437, 510,
    588, 672
};

seq_t a(idx_t n)
{
    /* Ensure the cache can hold index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* In practice use an assertion or error handler;
               for simplicity we abort here. */
            abort();
        }
        if (cache) {
            memcpy(new_cache, cache, cache_filled * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing entries up to n */
    for (idx_t i = cache_filled; i <= n; ++i) {
        seq_t val;
        if (i <= 16) {
            val = base_vals[i];
        } else {
            /* recurrence: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3)
                                      - 2*a(n-5) + 2*a(n-6)
                                      - 2*a(n-7) + a(n-8)   */
            val = (seq_t)2 * cache[i - 1];
            val -= (seq_t)2 * cache[i - 2];
            val += (seq_t)2 * cache[i - 3];
            val -= (seq_t)2 * cache[i - 5];
            val += (seq_t)2 * cache[i - 6];
            val -= (seq_t)2 * cache[i - 7];
            val += cache[i - 8];
        }
        cache[i] = val;
    }
    if ((size_t)n + 1 > cache_filled) {
        cache_filled = (size_t)n + 1;
    }
    return cache[n];
}
