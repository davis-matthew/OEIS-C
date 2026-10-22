/*
 * A328183  Expansion of e.g.f. 1 / (2 - exp(4*x)).
 * https://oeis.org/A328183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} 4^k * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache   = NULL; /* array of computed terms */
static size_t cache_sz = 0;   /* number of valid entries in cache */

/* Return the n-th term of sequence A328183 */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough for a(0) */
    if (n == 0) {
        if (cache_sz == 0) {
            cache_sz = 1;
            cache    = (seq_t*)malloc(sizeof(seq_t));
            cache[0] = (seq_t)1;
        }
        return cache[0];
    }

    /* Increase the allocated capacity if necessary */
    if (n + 1 > cache_sz) {
        size_t new_cap = cache_sz ? cache_sz * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_sz; i <= n; ++i) {
        if (i == 0) {                            /* should not happen, but keep it safe */
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum   = 0;                          /* running sum for a(i) */
        seq_t comb  = 0;                          /* current binomial coefficient */
        seq_t pow4  = 1;                          /* 4^k, k starts from 1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* binomial(i,k) via multiplicative recurrence */
            if (k == 1)
                comb = (seq_t)i;
            else
                comb = comb * (seq_t)(i - k + 1) / (seq_t)k;

            pow4 *= 4;                            /* pow4 = 4^k */

            /* term = a(i-k) * comb * pow4 */
            seq_t term = cache[i - k];
            term = term * comb;
            term = term * pow4;
            sum += term;
        }
        cache[i] = sum;
    }

    cache_sz = n + 1;   /* number of valid entries now */
    return cache[n];
}
