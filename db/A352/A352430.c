/*
 * A352430  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/5)} binomial(n,5*k+1) *
 *          a(n-5*k-1).
 * https://oeis.org/A352430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/5)} binomial(n,5*k+1) * a(n-5*k-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial term: a(0) = 1 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_idx(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);          /* allocation failure */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_cache_idx(n);
    if (cache[0] == 0)
        cache[0] = (seq_t)1;
    if (cache[n] != 0)
        return cache[n];

    seq_t result = 0;
    /* r is the current binomial argument (5*k+1) */
    idx_t r = 1;
    idx_t maxk = (n - 1) / 5;
    seq_t c = (seq_t)n;                 /* C(n,1) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* contribution of the k-th term */
        seq_t sub = a(n - r);
        result += c * sub;

        /* Prepare binomial for the next 5 steps, if any */
        if (k < maxk)
        {
            for (idx_t t = 1; t <= 5; ++t)
            {
                c = c * (seq_t)(n - (r + t - 1));
                c = c / (seq_t)(r + t);
            }
            r += 5;
        }
    }

    cache[n] = result;
    return result;
}
