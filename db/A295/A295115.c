/*
 * A295115  Number of n X 3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 4 1s.
 * https://oeis.org/A295115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 5*a(n-3) + 7*a(n-4) + 7*a(n-5) -
 *            a(n-6) - 8*a(n-7) - 5*a(n-8) + a(n-10) Initial values (offset
 *            1): a(1)=3, a(2)=9, a(3)=26, a(4)=77, a(5)=226, a(6)=665,
 *            a(7)=1960, a(8)=5769, a(9)=16983, a(10)=49998
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour below 1 irrelevant */

    static seq_t *cache = NULL;   /* memoised values */
    static idx_t cache_cap = 0;   /* allocated size (indexable up to cache_cap-1) */
    static idx_t cache_sz  = 0;   /* largest n already computed */
    static bool   init      = false;

    /* initialise base cases once */
    if (!init) {
        /* ensure space for indices 1..10 */
        size_t needed = 11;                 /* indices 0..10 inclusive */
        if (cache_cap < needed) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failed */
            cache = tmp;
            cache_cap = needed;
        }
        cache[1]  = (seq_t)3;
        cache[2]  = (seq_t)9;
        cache[3]  = (seq_t)26;
        cache[4]  = (seq_t)77;
        cache[5]  = (seq_t)226;
        cache[6]  = (seq_t)665;
        cache[7]  = (seq_t)1960;
        cache[8]  = (seq_t)5769;
        cache[9]  = (seq_t)16983;
        cache[10] = (seq_t)49998;
        cache_sz  = 10;
        init = true;
    }

    /* expand cache if needed for requested n */
    if (n >= cache_cap) {
        size_t needed = (size_t)n + 1;    /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        cache = tmp;
        cache_cap = needed;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        if (i <= 10) continue; /* base values already set */

        cache[i] = cache[i-1]
                 + (seq_t)3 * cache[i-2]
                 + (seq_t)5 * cache[i-3]
                 + (seq_t)7 * cache[i-4]
                 + (seq_t)7 * cache[i-5]
                 - cache[i-6]
                 - (seq_t)8 * cache[i-7]
                 - (seq_t)5 * cache[i-8]
                 + cache[i-10];
    }
    cache_sz = n;
    return cache[n];
}
