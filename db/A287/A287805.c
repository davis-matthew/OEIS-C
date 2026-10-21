/*
 * A287805  Number of quinary sequences of length n such that no two
 *          consecutive terms have distance 2.
 * https://oeis.org/A287805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 6*a(n-3) Base values: a(0)=1, a(1)=5,
 *            a(2)=19, a(3)=73
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_last = (idx_t)(-1);
    static size_t cache_cap  = 0;

    /* initialise cache with first four terms if needed */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)19;
        cache[3] = (seq_t)73;
        cache_last = 3;
    }

    /* if requested term already computed, return it */
    if (n <= (idx_t)cache_last)
        return cache[(size_t)n];

    /* enlarge allocation if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed – return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        cache[i] = 4*cache[i-1] + cache[i-2] - 6*cache[i-3];
    }
    cache_last = n;

    return cache[(size_t)n];
}
