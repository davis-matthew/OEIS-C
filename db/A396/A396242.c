/*
 * A396242  Expansion of 1 / (1 - B(B(x))), where B(x) = x/(1-x)^3.
 * https://oeis.org/A396242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 13*a(n-1) - 63*a(n-2) + 154*a(n-3) - 215*a(n-4)
 *            + 189*a(n-5) - 108*a(n-6) + 40*a(n-7) - 9*a(n-8) + a(n-9)
 *            Initial values are computed from the double‑sum formula
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/binomial.h"

/* Compute a single term via the double‑sum definition. */
static seq_t compute_term(idx_t m)
{
    if (m == 0)
        return (seq_t)1;
    seq_t total = (seq_t)0;
    for (idx_t i = 1; i <= m; ++i) {
        seq_t inner = (seq_t)0;
        seq_t bi = binomial(m + 2 * i - 1, 3 * i - 1);
        for (idx_t j = 1; j <= i; ++j) {
            seq_t bj = binomial(i + 2 * j - 1, 3 * j - 1);
            inner += bi * bj;
        }
        total += inner;
    }
    return total;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms      */
    static size_t cache_cap = 0;  /* allocated array capacity     */
    static idx_t  cache_used = 0; /* number of terms actually set */

    /* Prepare the cache and fill the first 10 terms from the
       double‑sum formula (indices 0 … 9).  The definition of a(n)
       requires these values for the recurrence. */
    if (cache == NULL) {
        cache_cap = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failure, unlikely for the tests */
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = compute_term(i);
        cache_used = 10;
    }

    /* If the requested index is already known, return it. */
    if (n < cache_used)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the linear recurrence. */
    for (idx_t i = cache_used; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos =  mul_small(cache[i - 1], 13);
        pos += mul_small(cache[i - 3], 154);
        pos += mul_small(cache[i - 5], 189);
        pos += mul_small(cache[i - 7], 40);
        pos += cache[i - 9];

        /* Negative contributions */
        seq_t neg = mul_small(cache[i - 2], 63);
        neg += mul_small(cache[i - 4], 215);
        neg += mul_small(cache[i - 6], 108);
        neg += mul_small(cache[i - 8], 9);

        cache[i] = pos - neg;
    }

    cache_used = n + 1;
    return cache[n];
}
