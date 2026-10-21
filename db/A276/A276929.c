/*
 * A276929  Number of ordered set partitions of [n] with at most nine elements
 *          per block.
 * https://oeis.org/A276929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276929: number of ordered set partitions of [n] with at most nine elements per block.
   Formula: a(0) = 1; for n ≥ 1:
        a(n) = Σ_{k=1}^{min(9,n)} binomial(n,k) * a(n−k)   (1)   */

/* binomial() is provided by the utilities package. */
#include <stdlib.h>
#include "../utils/binomial.h"

/* Cache to memoise previously computed values.  */
static seq_t  *cache     = NULL;   /* array of computed a(i) values          */
static size_t  cap       = 0;      /* allocated size of 'cache'              */
static size_t  len       = 0;      /* number of entries that have been computed */

/* Ensure that the cache has room for indices up to 'n' (inclusive).  */
static void
ensure_capacity(size_t n)
{
    if (n < cap)
        return;

    size_t new_cap = cap > 0 ? cap : 16;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */

    cache = tmp;
    /* Initialise any newly allocated entries to zero. */
    for (size_t i = cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;

    cap = new_cap;
}

/* Compute a(n) using the recurrence (1) and memoisation.  */
seq_t
a(idx_t n)
{
    /* Initialise cache and base case a(0)=1 on first call. */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();
        for (size_t i = 0; i < cap; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;
        len = 1;                     /* only a(0) computed so far */
    }

    /* Expand cache if needed. */
    ensure_capacity((size_t)n);

    /* Compute any missing terms up to n.  */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t s = (seq_t)0;
        size_t max_k = i < 9 ? i : 9;
        for (size_t k = 1; k <= max_k; ++k) {
            seq_t c = binomial((idx_t)i, (idx_t)k);
            s += c * cache[i - k];
        }
        cache[i] = s;
    }

    len = (size_t)n + 1;   /* all indices <= n are now computed */
    return cache[(size_t)n];
}
