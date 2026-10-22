/*
 * A311627  Coordination sequence Gal.6.190.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 3*a(n-3) - 3*a(n-4) + 4*a(n-5) -
 *            5*a(n-6) * + 6*a(n-7) - 6*a(n-8) + 6*a(n-9) - 5*a(n-10) +
 *            4*a(n-11) - 3*a(n-12) * + 3*a(n-13) - 3*a(n-14) + 2*a(n-15) -
 *            a(n-16) for n > 16 * Initial values: a(0)=1, a(1)=4, a(2)=8,
 *            a(3)=12, a(4)=18, a(5)=24, * a(6)=29, a(7)=32, a(8)=35...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL; /* array of computed terms */
    static size_t cache_size = 0;    /* number of values stored so far   */
    static size_t cache_cap  = 0;    /* current capacity of the cache    */

    /* coefficients of the 16‑term recurrence */
    static const int coeff[16] = {
        2, -3, 3, -3, 4, -5, 6, -6,
        6, -5, 4, -3, 3, -3, 2, -1
    };

    /* initial 17 terms for indices 0 … 16 */
    static const seq_t init[17] = {
        1, 4, 8, 12, 18,
        24, 29, 32, 35, 41,
        47, 52, 54, 56, 62,
        69, 76
    };

    /* initialise the cache on first call */
    if (!cache) {
        cache_cap = 17;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure → terminate */
        for (size_t i = 0; i < 17; ++i)
            cache[i] = init[i];
        cache_size = 17;
    }

    /* if the requested value is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* extend the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;                    /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure → terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        if (i <= 16) {                      /* already have the first 17 values */
            continue;
        }
        seq_t sum = 0;
        /* linear combination of the previous 16 terms */
        for (int k = 0; k < 16; ++k) {
            idx_t j = i - (k + 1);          /* term a(i-(k+1)) */
            sum += (seq_t)coeff[k] * cache[j];
        }
        cache[i] = sum;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
