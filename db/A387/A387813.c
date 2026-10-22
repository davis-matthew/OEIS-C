/*
 * A387813  a(n) = Sum_{k=0..n} binomial(5*n+4,5*k+1).
 * https://oeis.org/A387813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+4,5*k+1) Recurrence: a(n) =
 *            21*a(n-1) + 353*a(n-2) - 32*a(n-3) Initial values: a(0)=4,
 *            a(1)=93, a(2)=3381
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
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
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* malloc failed – abort */
            return (seq_t)0;
        }
        cache[0] = 4ULL;
        cache[1] = 93ULL;
        cache[2] = 3381ULL;
        cache_len = 3;
    }

    /* if requested index already computed, return it */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* realloc failed – abort */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 4ULL;
        } else if (i == 1) {
            val = 93ULL;
        } else if (i == 2) {
            val = 3381ULL;
        } else {
            val = 21 * cache[i - 1] + 353 * cache[i - 2] - 32 * cache[i - 3];
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
