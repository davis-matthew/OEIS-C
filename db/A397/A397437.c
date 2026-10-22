/*
 * A397437  Growth function of the reflection group of a four-dimensional
 *          orthoscheme with distinct edge lengths.
 * https://oeis.org/A397437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3), with initial values a(0)=1,
 *            a(1)=5, a(2)=14, a(3)=33
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache is large enough for indices 0..n */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
    } else if ((size_t)(n + 1) > (size_t)cache_size) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache_size = n + 1;
    }

    /* Initialize the base cases up to min(n,3) */
    if (n >= 0)            cache[0] = (seq_t)1;
    if (n >= 1)            cache[1] = (seq_t)5;
    if (n >= 2)            cache[2] = (seq_t)14;
    if (n >= 3)            cache[3] = (seq_t)33;

    /* Compute subsequent terms only if they haven’t been computed yet.
       We recompute the base cases each call, which is inexpensive. */
    for (idx_t i = 4; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + cache[i - 2] - cache[i - 3];
    }

    return cache[n];
}
