/*
 * A271485  Maximal term of TRIP-Stern sequence of level n corresponding to
 *          permutation triple (e,13,e).
 * https://oeis.org/A271485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-2) + a(n-4) - a(n-6) for n>5 with initial values
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=11
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
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
    static idx_t cache_cap   = 0;      /* amount of allocated space   */
    static idx_t computed_max = 0;     /* largest index already computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* initial allocation: 16 slots, more than the 6 seeds */
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            return 0;          /* out of memory – return 0 (unlikely in tests) */

        /* seed the first 6 terms */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 5;
        cache[4] = 7;
        cache[5] = 11;
        computed_max = 5;
    }

    /* return cached value if already computed */
    if (n <= computed_max)
        return cache[n];

    /* grow the cache array if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;            /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                  /* out of memory – return 0 */
        cache   = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* recurrence: 2*a(i-2)+a(i-4)-a(i-6) */
        seq_t t = cache[i-2] + cache[i-2];   /* 2 * a(i-2) */
        t = t + cache[i-4];
        cache[i] = t - cache[i-6];
    }

    computed_max = n;
    return cache[n];
}
