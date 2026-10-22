/*
 * A322925  Expansion of x*(1 + 2*x + 10*x^2)/((1 - x^2)*(1 - 10*x^2)).
 * https://oeis.org/A322925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322925: Expansion of x*(1 + 2*x + 10*x^2)/((1 - x^2)*(1 - 10*x^2)).
   Recurrence: a(0)=0, a(1)=1, a(2)=2, a(3)=21,
                a(n) = 11*a(n-2) - 10*a(n-4)  for n ≥ 4.     */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow any order of calls. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If the requested n is not in the cache, extend it. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;         /* allocation failed – very unlikely */
        cache = new_cache;

        /* Compute new terms starting from the first missing index. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)          cache[i] = (seq_t)0;
            else if (i == 1)     cache[i] = (seq_t)1;
            else if (i == 2)     cache[i] = (seq_t)2;
            else if (i == 3)     cache[i] = (seq_t)21;
            else                 cache[i] = 11 * cache[i-2] - 10 * cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
