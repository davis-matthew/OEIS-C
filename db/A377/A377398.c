/*
 * A377398  Expansion of e.g.f. (2 - exp(x))^3.
 * https://oeis.org/A377398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-3, a(2)=3; a(n)=5*a(n-1)-6*a(n-2)-24 for n>=3
   Closed form for n>0: a(n)=3*2^(n+1)-3^n-12 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache for arbitrary order calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise base values */
    if (cache_len == 0) {
        cache_len = 3;                     /* indices 0,1,2 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed (unlikely in tests) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)3;
    }

    /* if requested index already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache to include up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        return 0;                          /* allocation failed */
    }
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] - 6 * cache[i-2] - 24;
    }
    cache_len = new_len;

    return cache[n];
}
