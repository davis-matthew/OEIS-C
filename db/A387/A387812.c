/*
 * A387812  a(n) = Sum_{k=0..n} binomial(5*n+3,5*k+1).
 * https://oeis.org/A387812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387812
   a(n) = 21*a(n-1) + 353*a(n-2) - 32*a(n-3)
   a(0) = 3,  a(1) = 36,  a(2) = 1807
   (This recurrence holds for all n ≥ 3.) */

#include <stdlib.h>
#include <stddef.h>

/* -------------  Private static cache  ------------- */
static seq_t *cache = NULL;        /* array of computed values   */
static size_t cache_alloc = 0;     /* allocated size (in elements) */
static idx_t  next_computed = 0;   /* first index not yet computed */

/* Ensure the cache can hold indices up to (needed-1). */
static void ensure_cache(size_t needed)
{
    if (cache_alloc >= needed)
        return;

    size_t new_alloc = cache_alloc ? (cache_alloc << 1) : 4;
    if (new_alloc < needed)
        new_alloc = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    if (!tmp)  /* allocation failed – leave cache unchanged */
        return;

    cache = tmp;
    cache_alloc = new_alloc;
}

/* -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_alloc = 4;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* SAFETY – unlikely to happen */

        /* seed the first three terms */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)1807;
        next_computed = 3;   /* next free index */
    }

    /* If desired term already computed, return it */
    if (n < next_computed)
        return cache[n];

    /* Grow cache if necessary */
    ensure_cache((size_t)n + 1);

    /* Compute missing terms up to n */
    for (idx_t i = next_computed; i <= n; ++i) {
        seq_t val = cache[i-1] * (seq_t)21;
        val += cache[i-2] * (seq_t)353;
        val -= cache[i-3] * (seq_t)32;
        cache[i] = val;
    }
    next_computed = n + 1;
    return cache[n];
}
