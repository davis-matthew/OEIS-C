/*
 * A368455  Expansion of e.g.f. 3*exp(-x) / (4 - exp(3*x)).
 * https://oeis.org/A368455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=1..n} 3^(k-1) * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1 */

#include <stdlib.h>
/* seq_t and idx_t are provided by the build environment */

static seq_t *cache = NULL;          /* memoisation array for a(n) */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t max_computed = 0;       /* highest n computed (inclusive) */

/* Allocate or grow the cache so it can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – terminate program */
        exit(EXIT_FAILURE);
    cache = tmp;
    /* zero out the new area so that default values are known (unused after tiny_n) */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache if this is the first call */
    if (!cache)
    {
        ensure_cache(0);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        max_computed = 0;
    }

    /* Compute up to the requested index if necessary */
    while (max_computed < n)
    {
        idx_t i = max_computed + 1;          /* index to compute */
        ensure_cache(i);                    /* make room for a[i] */

        seq_t sum  = 0;
        seq_t binom = 1;                     /* binomial(n,k-1) before first iteration */
        seq_t power = 1;                     /* 3^(k-1) for current k */

        /* k goes from 1 to i inclusive */
        for (idx_t k = 1; k <= i; ++k)
        {
            /* Update binomial coefficient: binom(n,k) = binom(n,k-1)*(n-k+1)/k */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            /* Add term: binom * 3^(k-1) * a(i-k) */
            sum += binom * power * cache[i - k];

            /* Prepare power for next iteration: 3^(k) */
            power *= (seq_t)3;
        }

        /* Add the (-1)^i part */
        seq_t result = sum;
        if (i & 1)          /* i odd: subtract 1 */
            result -= (seq_t)1;
        else                /* i even: add 1 */
            result += (seq_t)1;

        cache[i] = result;
        max_computed = i;
    }

    return cache[n];
}
