/*
 * A257273  a(n) = 2^(n-1)*(2^n+3).
 * https://oeis.org/A257273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2, a(1) = 5, a(n) = 6*a(n-1) - 8*a(n-2) for n>=2 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Grow cache to accommodate index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        if (cache)
            free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize first two terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache_len = 1;
    }
    if (cache_len == 1 && cache_cap >= 2) {
        cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* Compute missing terms using the recurrence */
    while (cache_len <= (size_t)n) {
        seq_t a_prev1 = cache[cache_len - 1];
        seq_t a_prev2 = cache[cache_len - 2];
        cache[cache_len] = (seq_t)6 * a_prev1 - (seq_t)8 * a_prev2;
        ++cache_len;
    }

    return cache[(size_t)n];
}
