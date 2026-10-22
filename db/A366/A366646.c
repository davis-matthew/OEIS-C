/*
 * A366646  G.f. A(x) satisfies A(x) = 1 + (x * A(x) / (1 - x))^4.
 * https://oeis.org/A366646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n-1,n-4k) * binomial(4k,k) /
 *            (3k+1)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to compute binomial coefficients */
#include "../utils/binomial.h"

#include <stdlib.h>   /* realloc, exit */

static seq_t *cache = NULL;        /* array of computed values */
static size_t cache_size = 0;      /* number of elements stored (next free index) */
static size_t cache_cap  = 0;      /* current allocation size */

/* Ensure that the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size) return;   /* already computed */

    /* Grow the cache if necessary.  Double the capacity until it fits n. */
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {                  /* allocation failure – abort */
            exit(EXIT_FAILURE);
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new values sequentially. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[i] = (seq_t)1;                 /* a(0) = 1 */
        }
        else if (i < 4)
        {
            cache[i] = (seq_t)0;                 /* a(1)=a(2)=a(3)=0 */
        }
        else
        {
            seq_t sum = (seq_t)0;
            idx_t maxk = i / 4;                  /* floor(i/4) */
            for (idx_t k = 1; k <= maxk; ++k)
            {
                seq_t b1 = binomial(i - 1, i - 4 * k); /* binomial(i-1, i-4k) */
                seq_t b2 = binomial(4 * k, k);        /* binomial(4k, k) */
                seq_t term = (b1 * b2) / (seq_t)(3 * k + 1);
                sum += term;
            }
            cache[i] = sum;
        }
    }

    cache_size = (size_t)n + 1;
}

/* Public sequence function required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
