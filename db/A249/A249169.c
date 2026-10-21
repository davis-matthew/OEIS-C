/*
 * A249169  Fibonacci 16-step numbers, a(n) = a(n-1) + a(n-2) + ... + a(n-16).
 * https://oeis.org/A249169
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + … + a(n-16) (offset 15, a(15) = 1,
 *            a(16) = 1, …)
 * Range:     u8 n <= 23, u16 n <= 32, u32 n <= 48, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] := a(i), for i >= 15.
       The array may grow to contain all indices up to the
       largest n requested.  cache_size represents one past
       the largest computed index. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated number of entries */
    static idx_t cache_size = 0;     /* one past largest computed index */

    if (n < 15) 
        return 0;     /* undefined for the sequence */

    /* Ensure we have storage for index n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to and including n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 15) {
            cache[15] = 1;
            continue;
        }
        seq_t sum = 0;
        for (int k = 1; k <= 16; ++k) {
            idx_t idx = i - k;
            if (idx >= 15)
                sum += cache[(size_t)idx];
        }
        cache[(size_t)i] = sum;
    }
    cache_size = n + 1;

    return cache[(size_t)n];
}
