/*
 * A352904  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/4)} binomial(n,4*k+1) *
 *          a(k).
 * https://oeis.org/A352904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/4⌋} binomial(n,4k+1) * a(k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that cache has space for indices up to n. */
static void
grow_cache(idx_t n)
{
    if ((idx_t)cache_cap > n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort(); /* memory allocation failed */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main recursive function with memoisation. */
seq_t
a(idx_t n)
{
    grow_cache(n);

    if (n == 0) {
        if (cache[0] == 0)
            cache[0] = 1;
        return cache[0];
    }

    if (cache[n] != 0)
        return cache[n];

    seq_t sum = 0;
    idx_t maxk = (n - 1) / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t col = 4 * k + 1;
        seq_t term = binomial(n, col);
        term = term * a(k);
        sum = sum + term;
    }

    cache[n] = sum;
    return sum;
}
