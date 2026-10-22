/*
 * A394644  Expansion of e.g.f. exp((4/3)*x^3+x).
 * https://oeis.org/A394644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 1, a(2) = 1
   a(n) = a(n-1) + 4*(n-1)*(n-2)*a(n-3)   for n ≥ 3 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values (0 … cache_size-1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */

    if (n >= cache_size) {
        /* Extend the cache up to n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache,
                                     (size_t)new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) abort();   /* allocation failure */
        cache = tmp;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 3) {
                cache[i] = (seq_t)1;
            } else {
                seq_t prod = (seq_t)4 * (seq_t)(i - 1)
                                * (seq_t)(i - 2)
                                * cache[i - 3];
                cache[i] = cache[i - 1] + prod;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
