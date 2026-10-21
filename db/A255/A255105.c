/*
 * A255105  Number of length n+6 0..2 arrays with at most one downstep in
 *          every 6 consecutive neighbor pairs.
 * https://oeis.org/A255105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -3*a(n-2) +a(n-3) +25*a(n-6) -42*a(n-7)
 *            +18*a(n-8) -10*a(n-12) +6*a(n-13)
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1..13 */
    static const seq_t init[14] = {
        0, 540, 1008, 2032, 4338, 9297, 19263, 38010, 73278,
        143045, 288057, 594045, 1228136, 2495244
    };

    if (n <= 13)
        return init[n];

    /* static cache for memoisation */
    static seq_t *cache      = NULL;  /* array of computed values */
    static idx_t cache_size   = 0;    /* allocated array length */
    static idx_t max_computed = 0;    /* highest n that has been computed */

    /* initialise cache the first time */
    if (!cache) {
        cache_size = 14;                 /* indices 0..13 are required */
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache)
            return (seq_t)0;             /* out of memory – fallback */
        for (idx_t i = 1; i < 14; ++i)
            cache[i] = init[i];
        max_computed = 13;
    }

    if (n > max_computed) {
        /* grow cache if necessary */
        size_t new_size = (size_t)(n + 1);   /* index up to n inclusive */
        if (new_size > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;             /* realloc failed – fallback */
            cache       = tmp;
            cache_size  = new_size;
        }

        /* compute values forward from max_computed + 1 to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t pos = 3 * cache[i - 1] + cache[i - 3]
                      + 25 * cache[i - 6] + 18 * cache[i - 8]
                      + 6 * cache[i - 13];

            seq_t neg = 3 * cache[i - 2] + 42 * cache[i - 7]
                      + 10 * cache[i - 12];

            cache[i] = pos - neg;             /* pos ≥ neg for all n ≥ 14 */
        }
        max_computed = n;
    }

    return cache[n];
}
