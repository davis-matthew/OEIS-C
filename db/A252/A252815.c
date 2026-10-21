/*
 * A252815  Number of n X 3 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its city block distance from the upper
 *          left and every value increasing by 0 or 1 with every step right or
 *          down.
 * https://oeis.org/A252815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n>7 Initial values: a(1)=4, a(2)=17,
 *            a(3)=63, a(4)=187, a(5)=468, a(6)=1032, a(7)=2067 Computes any
 *            a(n) for n≥1. Uses memoisation for efficiency
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 104, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms, 0‑based: a(1)=cache[0] */
    static idx_t cache_len = 0;   /* number of terms stored (cache_len >=7 after init) */

    if (n < 1)          /* out of defined range – undefined behaviour */
        return 0;

    /* initialise cache with first 7 values on first call */
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed – return 0 as a safe fallback */
        cache[0] = 4;
        cache[1] = 17;
        cache[2] = 63;
        cache[3] = 187;
        cache[4] = 468;
        cache[5] = 1032;
        cache[6] = 2067;
    }

    /* if requested term already in cache, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow cache to hold terms up to n */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)          /* allocation failed – return 0 as a safe fallback */
        return 0;
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is a 0‑based index, so a(i+1) = cache[i] */
        seq_t pos = (seq_t)7 * cache[i - 1];
        pos += (seq_t)35 * cache[i - 3];
        pos += (seq_t)21 * cache[i - 5];
        pos += cache[i - 7];

        seq_t neg = (seq_t)21 * cache[i - 2];
        neg += (seq_t)35 * cache[i - 4];
        neg += (seq_t)7  * cache[i - 6];

        cache[i] = pos - neg;
    }

    cache_len = n;
    return cache[n - 1];
}
