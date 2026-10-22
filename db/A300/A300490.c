/*
 * A300490  Expansion of e.g.f. -exp(-x)*log(1 - x)/(1 - x).
 * https://oeis.org/A300490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=5, a(4)=20, a(n+4) = (2*n+5)*a(n+3)
 *            - (n^2+2*n-1)*a(n+2) - (2*n+3)*(n+2)*a(n+1) - (n+2)*(n+1)*a(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
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
    static idx_t cache_len = 0;
    idx_t i;

    /* initialise cache with first 5 terms if not already done */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 5);
        if (!cache) return 0;          /* out of memory: safe fallback */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 5;
        cache[4] = 20;
        cache_len = 5;
    }

    /* if the requested index is not yet computed, extend cache */
    if (n >= cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;      /* out of memory: safe fallback */
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (i = cache_len; i <= n; ++i) {
            /* recurrence is valid for indices ≥ 5 */
            idx_t m = i - 4;                       /* n in the recurrence */
            seq_t coeff1 = (seq_t)(2 * m + 5);
            seq_t coeff2 = (seq_t)(m * m + 2 * m - 1);
            seq_t coeff3 = (seq_t)((2 * m + 3) * (m + 2));
            seq_t coeff4 = (seq_t)((m + 2) * (m + 1));

            cache[i] =  coeff1 * cache[i - 1]
                     - coeff2 * cache[i - 2]
                     - coeff3 * cache[i - 3]
                     - coeff4 * cache[i - 4];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
