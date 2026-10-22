/*
 * A391835  a(n) = (1/4) * Sum_{k=0..n} (k+2) * 2^(n-k) *
 *          binomial(2*k+2,2*n-2*k+1).
 * https://oeis.org/A391835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Σ_{k=0..n} (k+2) * 2^(n-k) * binomial(2*k+2,
 *            2*n-2*k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the 8‑term linear recurrence:                      */
/* a(n) = 4 a(n-1) + 2 a(n-2) - 4 a(n-3) - 33 a(n-4) - 8 a(n-5)  */
/*        + 8 a(n-6) + 32 a(n-7) - 16 a(n-8) (n ≥ 8)               */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least (n+1) terms.  */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache with the first 8 terms on first call. */
    if (cache_cap == 0)
    {
        ensure_cache(7);          /* allocate 8 slots */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)50;
        cache[4] = (seq_t)179;
        cache[5] = (seq_t)661;
        cache[6] = (seq_t)2390;
        cache[7] = (seq_t)8476;
    }

    /* Return cached value if already computed. */
    if (n < cache_cap)
        return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_cap; i <= n; ++i)
    {
        if (i < 8)
        {
            /* Base terms already set; this block is never executed. */
            continue;
        }

        seq_t val = (seq_t)4 * cache[i-1]
                  + (seq_t)2 * cache[i-2]
                  - ( (seq_t)4 * cache[i-3] )
                  - ( (seq_t)33 * cache[i-4] )
                  - ( (seq_t)8 * cache[i-5] )
                  + ( (seq_t)8 * cache[i-6] )
                  + ( (seq_t)32 * cache[i-7] )
                  - ( (seq_t)16 * cache[i-8] );

        ensure_cache(i);
        cache[i] = val;
    }

    return cache[n];
}
