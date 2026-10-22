/*
 * A399560  G.f. A(x) satisfies: A(x) = (1 - 2*x^2 + x*A(2*x)) / (1 - x^2 -
 *          x^3).
 * https://oeis.org/A399560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n < 3; a(n) = 2^(n-1)*a(n-1) + a(n-2) + a(n-3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build environment. */

/* Static cache for previously computed terms. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure the cache is large enough for indices [0, needed-1]. */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();                 /* memory allocation failure */
    /* Zero-initialize the newly allocated part. */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        newp[i] = 0;
    cache = newp;
    cache_cap = new_cap;
}

/* Computes the nth term of sequence A399560. */
seq_t a(idx_t n)
{
    /* Initial terms a(0)=a(1)=a(2)=1. */
    ensure_cache(3);
    cache[0] = cache[1] = cache[2] = (seq_t)1;

    /* Compute up to n if necessary. */
    ensure_cache(n + 1);
    if (n < 3)
        return cache[n];

    /* Use the recurrence: a(n) = 2^(n-1)*a(n-1) + a(n-2) + a(n-3). */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] != 0)          /* already computed in a previous call */
            continue;
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t shift;
        /* Avoid undefined behaviour for shifts >= width of seq_t. */
        if ((i - 1) < (sizeof(seq_t) * 8))
            shift = a1 << (i - 1);  /* 2^(i-1) * a(i-1)  */
        else
            shift = 0;              /* product would overflow; term doesn't fit in seq_t anyway */

        cache[i] = shift + a2 + a3;
    }
    return cache[n];
}
