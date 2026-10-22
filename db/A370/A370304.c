/*
 * A370304  Number of multiplex juggling cards with n balls and capacity 2.
 * https://oeis.org/A370304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a(0)=1, a(1)=2, a(2)=7, a(3)=17, a(4)=41, a(5)=91
 *            and for n ≥ 6: a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6)
 *            (G.f.: (1 - x + x² + x³)/(1 - x - x²)³)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with first 6 terms */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)41;
        cache[5] = (seq_t)91;
    }

    /* If requested index already computed, return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Enlarge cache to accommodate up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failed */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (size_t i = (size_t)cache_size; i < (size_t)new_size; ++i) {
        /* For i ≥ 6 the recurrence is valid */
        seq_t term = (seq_t)3 * cache[i - 1];
        term -= (seq_t)5 * cache[i - 3];
        term += (seq_t)3 * cache[i - 5];
        term += cache[i - 6];
        cache[i] = term;
    }
    cache_size = new_size;

    return cache[(size_t)n];
}
