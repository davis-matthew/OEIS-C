/*
 * A264294  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,1 0,-2 1,0 or -1,0.
 * https://oeis.org/A264294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + 4*a(n-2) + 5*a(n-3) - 5*a(n-4) - 5*a(n-5) 
 *            + 4*a(n-6) - a(n-7) - a(n-8)
 * Base values (offset 1): 
 *   a(1)=2, a(2)=9, a(3)=20, a(4)=56, a(5)=169, a(6)=441, a(7)=1258, a(8)=3520
 */

#include <stdlib.h>

static seq_t *cache  = NULL;   /* 1‑based cache, index 0 unused          */
static size_t  cap   = 0;       /* allocated capacity                     */
static idx_t   used  = 0;       /* largest n for which cache[n] is set    */

/* Ensure cache holds values 1 … n (inclusive).  On the first call
 * initialise base values for 1 … 8; after that extend the cache as
 * needed and compute missing terms via the recurrence. */
static void ensure_cache(idx_t n)
{
    /* If we already have n, nothing to do. */
    if (used >= n)
        return;

    /* Allocate and initialise base values on first use. */
    if (cache == NULL) {
        /* Start with sufficient capacity for small n. */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)               /* allocation failed */
            return;

        /* Zero unused slot 0 (not part of the sequence). */
        cache[0] = (seq_t)0;

        /* Base values 1 … 8. */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)56;
        cache[5] = (seq_t)169;
        cache[6] = (seq_t)441;
        cache[7] = (seq_t)1258;
        cache[8] = (seq_t)3520;

        used = 8;   /* we now have terms up to 8 */
    }

    /* Grow the cache if the requested index is beyond current capacity. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return;
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms using the recurrence. */
    for (idx_t i = used + 1; i <= n; ++i) {
        seq_t v = cache[(size_t)(i - 1)];
        v += (seq_t)4 * cache[(size_t)(i - 2)];
        v += (seq_t)5 * cache[(size_t)(i - 3)];
        v -= (seq_t)5 * cache[(size_t)(i - 4)];
        v -= (seq_t)5 * cache[(size_t)(i - 5)];
        v += (seq_t)4 * cache[(size_t)(i - 6)];
        v -= cache[(size_t)(i - 7)];
        v -= cache[(size_t)(i - 8)];
        cache[(size_t)i] = v;
    }

    /* Update the largest computed index. */
    if (used < n)
        used = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    ensure_cache(n);
    return cache[(size_t)n];
}
