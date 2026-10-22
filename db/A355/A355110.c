/*
 * A355110  Expansion of e.g.f. 2 / (3 - 2*x - exp(2*x)).
 * https://oeis.org/A355110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = n*a(n-1) + Sum_{k=1..n}
 *            binomial(n,k)*2^(k-1)*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;   /* memoised sequence values */
static idx_t  cache_len = 0;  /* number of values currently stored */
static idx_t  cache_cap = 0;  /* array capacity */

/* Grow the cache so that it can hold at least (n+1) entries. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence, memoising intermediate results. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1; /* a(0) = 1 */
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1; /* binomial(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient to binomial(i,k) */
            bin = bin * (seq_t)(i - k + 1) / (seq_t)k;

            /* 2^(k-1) */
            seq_t p2 = pow2(k - 1);

            /* Contribution to the sum */
            seq_t temp = bin * p2 * cache[i - k];
            sum += temp;
        }

        cache[i] = (seq_t)i * cache[i - 1] + sum;
    }

    cache_len = n + 1;
    return cache[n];
}
