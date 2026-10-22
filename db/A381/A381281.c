/*
 * A381281  Expansion of e.g.f. 1/(1 - x * cosh(3*x)).
 * https://oeis.org/A381281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} 9^k * (2*k+1) *
 *            binomial(n,2*k+1) * a(n-2*k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization array */
static idx_t  cache_size = 0;        /* highest index computed */
static idx_t  cache_cap   = 0;        /* allocated capacity */

/* Ensure the cache has space for indices up to n */
static void _ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1); /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* compute a(n) with memoization */
seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
    {
        _ensure_cache(0);
        if (cache_size == 0)
        {
            cache[0] = 1;
            cache_size = 1;
        }
        return cache[0];
    }

    _ensure_cache(n);

    /* Compute missing entries up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        idx_t max_k = (i - 1) / 2;
        seq_t sum = 0;

        /* Initialization for k=0 */
        seq_t bin   = (seq_t)i;   /* C(i, 1) */
        seq_t nine  = 1;          /* 9^k, with k=0 */

        for (idx_t k = 0; k <= max_k; ++k)
        {
            idx_t r = 2 * k + 1;             /* 2*k+1 */
            seq_t term = cache[i - r];
            term = term * bin;              /* * C(i,r) */
            term = term * (seq_t)(2 * k + 1);/* * (2*k+1) */
            term = term * nine;             /* * 9^k */
            sum += term;

            if (k < max_k)
            {
                /* Update bin to C(i, r+2) */
                seq_t num1 = (seq_t)(i - r);
                seq_t num2 = (seq_t)(i - r - 1);
                seq_t den  = (seq_t)(r + 1) * (seq_t)(r + 2);
                bin = bin * num1 * num2 / den;

                nine *= 9; /* next power of 9 */
            }
        }

        cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
