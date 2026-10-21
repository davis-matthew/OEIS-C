/*
 * A253711  Second partial sums of 11th powers (A008455).
 * https://oeis.org/A253711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + n^11 Initial values: a(1) = 1, a(2) =
 *            2050
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Cache of already computed terms. Indexes start at 1. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that terms up to n are cached. If n is already known, nothing
   is done.  The function allocates and fills the cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;           /* nothing to do */

    idx_t old_size = cache_size;  /* maximum previously computed */

    /* Allocate space up to index n (allocate +1 so index n is valid). */
    size_t new_alloc = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;

    /* Initialise base terms if they were not present before. */
    if (old_size < 1 && n >= 1)
        cache[1] = (seq_t)1;
    if (old_size < 2 && n >= 2)
        cache[2] = (seq_t)2050;

    /* Compute missing terms sequentially.  For k > old_size we have
       all required previous values. */
    idx_t start = (old_size >= 2) ? old_size : 2;
    for (idx_t k = start + 1; k <= n; ++k) {
        seq_t pow = big_pow((seq_t)k, (seq_t)11);
        seq_t prev  = cache[k-1];
        seq_t prevp = cache[k-2];
        cache[k] = prev * (seq_t)2 - prevp + pow;
    }

    cache_size = n;
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset is 1; undefined below */
    ensure_cache(n);
    return cache[n];
}
