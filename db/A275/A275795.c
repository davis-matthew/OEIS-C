/*
 * A275795  The x members of the positive proper solutions (x = x2(n), y =
 *          y2(n)) of the second class for the Pell equation x^2 - 2*y^2 =
 *          +7^2.
 * https://oeis.org/A275795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), with a(-1) = 9, a(0) = 11
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 921 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries stored            */
    static size_t cache_cap = 0;  /* current capacity of cache array      */

    /* Grow the cache array if it cannot hold the requested index. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first element if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)11;            /* a(0) */
        cache_len = 1;
    }

    /* Compute any missing values up to n. */
    if ((size_t)n >= cache_len) {
        seq_t prevprev = (seq_t)9;       /* a(-1) */
        seq_t prev     = cache[0];       /* a(0) */
        idx_t idx = 1;
        while (idx <= n) {
            seq_t cur = (seq_t)6 * prev - prevprev;
            cache[idx] = cur;
            prevprev = prev;
            prev = cur;
            ++idx;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
