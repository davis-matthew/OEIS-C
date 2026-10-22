/*
 * A375631  Expansion of e.g.f. exp(2*x) / (1 - x^2/2 * exp(x)).
 * https://oeis.org/A375631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+2)^(n-2k)/(2^k*(n-2k)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   term_for
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/term_for.h"

/* Simple cache for already computed a(n) values.
   0 is a valid flag that the value has not yet been computed
   (the sequence never produces zero). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* initialise newly allocated slots with 0 */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

/* Main entry point for the OEIS sequence A375631. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache[(size_t)n] != (seq_t)0)
        return cache[(size_t)n];

    seq_t sum = (seq_t)0;
    idx_t k_max = n / 2;
    for (idx_t k = 0; k <= k_max; ++k)
        sum += term_for(n, k);

    cache[(size_t)n] = sum;
    return sum;
}
