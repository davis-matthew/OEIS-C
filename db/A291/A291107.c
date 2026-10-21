/*
 * A291107  Number of irredundant sets in the n-pan graph.
 * https://oeis.org/A291107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-6) A(1)=3, A(2)=5, A(3)=7,
 *            A(4)=16, A(5)=26, A(6)=42
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL; /* 1‑indexed array */
static idx_t  cache_cap  = 0;    /* allocated size of cache      */
static idx_t  max_computed = 0;  /* highest index that has been computed */

/* Ensure that terms up to n are available, computing any missing values. */
static void ensure_cache(idx_t n)
{
    if (n < 1)
        return;                         /* nothing to do for n <= 0 */

    if (n <= max_computed)          /* everything already computed */
        return;

    /* Grow the cache if necessary */
    if (n + 1 > cache_cap) {
        seq_t *new_cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache)
            exit(1);                  /* out of memory */
        /* copy existing values */
        for (idx_t i = 1; i <= max_computed; ++i)
            new_cache[i] = cache[i];
        /* free old array */
        free(cache);
        cache = new_cache;
        cache_cap = (size_t)(n + 1);
    }

    /* Compute missing terms using the recurrence */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1)
            val = (seq_t)3;
        else if (k == 2)
            val = (seq_t)5;
        else if (k == 3)
            val = (seq_t)7;
        else if (k == 4)
            val = (seq_t)16;
        else if (k == 5)
            val = (seq_t)26;
        else if (k == 6)
            val = (seq_t)42;
        else
            val = cache[k-1] + cache[k-2] + cache[k-4] - cache[k-6];
        cache[k] = val;
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined below offset */
    ensure_cache(n);
    return cache[n];
}
