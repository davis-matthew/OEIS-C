/*
 * A275507  Number of 7 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 18*a(n-2) + 11*a(n-3) - 23*a(n-4) - 4*a(n-5) *
 *            for n > 6 * Initial values: * a(1) = 48, a(2) = 200, a(3) =
 *            1168, a(4) = 7140, * a(5) = 43780, a(6) = 268152 * Computation
 *            is memoized, so repeated or out‑of‑order calls are O(1) * after
 *            the first computation
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized values, 1‑based index */
static size_t cache_cap = 0;     /* size allocated for cache[0..cap-1] */
static idx_t  cached_until = 0;  /* highest n cached so far */

/* Ensure the cache can hold values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n + 1)
    {
        seq_t *newp = (seq_t *)realloc(cache,
                                      ((size_t)n + 1) * sizeof(seq_t));
        if (!newp)
            exit(EXIT_FAILURE); /* fatal if allocation fails */
        cache = newp;
        cache_cap = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    /* Base cases: known initial values */
    switch (n)
    {
        case 1: return (seq_t)48;
        case 2: return (seq_t)200;
        case 3: return (seq_t)1168;
        case 4: return (seq_t)7140;
        case 5: return (seq_t)43780;
        case 6: return (seq_t)268152;
        default: break;
    }

    /* Ensure cache is allocated for requested index */
    ensure_cache(n);

    /* If we haven't yet computed up to n, do it now */
    if (cached_until < n)
    {
        /* Initialize the first six entries if necessary */
        if (cached_until < 6)
        {
            static const seq_t init[6] =
                {48, 200, 1168, 7140, 43780, 268152};
            size_t start = cached_until + 1;
            for (idx_t i = start; i <= 6; ++i)
                cache[i] = init[i - 1];
            cached_until = 6;
        }

        /* Compute remaining terms up to n */
        for (idx_t i = cached_until + 1; i <= n; ++i)
        {
            if (i <= 6) /* safety, though should never happen here */
                cache[i] = (seq_t)0;
            else
            {
                cache[i] =
                    (seq_t)3 * cache[i - 1] +
                    (seq_t)18 * cache[i - 2] +
                    (seq_t)11 * cache[i - 3] -
                    (seq_t)23 * cache[i - 4] -
                    (seq_t)4 * cache[i - 5];
            }
        }
        cached_until = n;
    }

    return cache[n];
}
