/*
 * A360088  a(n) = Sum_{k=0..n} (-1)^k * binomial(4*k,n-k).
 * https://oeis.org/A360088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(4*k, n-k) Recurrence: a(n)
 *            = -a(n-1) - 4*a(n-2) - 6*a(n-3) - 4*a(n-4) - a(n-5) Initial
 *            values: a(0)=1, a(1)=-1, a(2)=-3, a(3)=1, a(4)=13
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 32, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* number of elements stored */

    /* create base entries if cache is empty */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 5);
        if (cache == NULL)
            return (seq_t)0; /* allocation error – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-3;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)13;
        cache_cap = 5;
    }

    /* if the wanted term is already computed, return it */
    if ((idx_t)cache_cap > n)
        return cache[n];

    /* otherwise grow the cache to size n+1 */
    size_t old_cap = cache_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (new_cache == NULL)
        return (seq_t)0; /* allocation error */
    cache = new_cache;
    cache_cap = n + 1;

    /* compute missing terms using the recurrence */
    for (idx_t i = old_cap; i <= n; ++i) {
        if (i <= 4) {
            /* base cases already set */
            continue;
        }
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        seq_t v4 = cache[i - 4];
        seq_t v5 = cache[i - 5];
        cache[i] = -v1 - (seq_t)4 * v2 - (seq_t)6 * v3 -
                   (seq_t)4 * v4 - v5;
    }

    return cache[n];
}
