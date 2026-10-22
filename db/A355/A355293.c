/*
 * A355293  Expansion of e.g.f. 1 / (1 - x - x^2/2 - x^3/3).
 * https://oeis.org/A355293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n*(n-1)*a(n-2)/2 + n*(n-1)*(n-2)*a(n-3)/3 a(0)
 *            = 1, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize base values */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache to hold up to n */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!cache) return (seq_t)0; /* allocation failed (unlikely) */

    /* compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t nis = (seq_t)i;
        seq_t term;

        if (i == 0) { term = (seq_t)1; }
        else if (i == 1) { term = (seq_t)1; }
        else if (i == 2) { term = (seq_t)3; }
        else {
            term = nis * cache[i - 1];
            term += nis * (nis - 1) * cache[i - 2] / (seq_t)2;
            term += nis * (nis - 1) * (nis - 2) * cache[i - 3] / (seq_t)3;
        }

        cache[i] = term;
    }

    cache_len = n + 1;
    return cache[n];
}
