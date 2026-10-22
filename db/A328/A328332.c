/*
 * A328332  Expansion of (1 + 4*x - 5*x^2 + 10*x^3) / ((1 - x) * (1 -
 *          10*x^2)).
 * https://oeis.org/A328332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-2) - 10*a(n-3) (for n ≥ 4) * Initial
 *            values: a(0)=1, a(1)=5, a(2)=10, a(3)=60
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute the n-th term of OEIS A328332 */
seq_t a(idx_t n)
{
    /* initialise the cache on the first call */
    if (cache_len == 0) {
        cache_len = 4;                          /* store a(0)..a(3) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)60;                  /* a(3) from the generating function */
    }

    /* if already computed, return */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to hold up to index n */
    if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        /* compute missing terms via the recurrence, starting at a(4) */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = (seq_t)10 * cache[i - 2];
            seq_t t2 = (seq_t)10 * cache[i - 3];
            cache[i] = cache[i - 1] + t1 - t2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
