/*
 * A244430  E.g.f.: exp( Sum_{n>=1} Fibonacci(n)*x^n/n ).
 * https://oeis.org/A244430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1, a(1) = 1;
 * for n ≥ 2: a(n) = n*a(n-1) + (n-1)*(n-2)*a(n-2).
 * This implements the recurrent formula given in OEIS A244430.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_seq_array.h"   /* provides grow_seq_array() */

static seq_t *cache = NULL;   /* cache of previously computed terms */
static idx_t  cache_size = 0; /* current number of stored terms */
static size_t cache_cap  = 0; /* allocated capacity in terms of seq_t elements */

/* Extend the cache to hold terms up to 'up_to' (inclusive). */
static void
ensure_cache(idx_t up_to)
{
    if (cache_size <= up_to)
    {
        idx_t const old_size = cache_size;
        /* grow_seq_array will increase cache_cap to at least up_to+1
           and set cache_size accordingly. */
        grow_seq_array(&cache, &cache_cap, up_to + 1);   /* cache_cap becomes up_to+1 */
        cache_size = up_to + 1;

        /* Initialize the base values if they are missing. */
        if (old_size == 0) {
            cache[0] = (seq_t)1;
            if (cache_size > 1) cache[1] = (seq_t)1;
        } else if (old_size == 1) {
            cache[1] = (seq_t)1;
        }

        /* Compute any new terms using the recurrence. */
        idx_t const start = old_size > 1 ? old_size : 2;
        for (idx_t i = start; i < cache_size; ++i) {
            seq_t term1 = ((seq_t)i) * cache[i - 1];
            seq_t term2 = ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * cache[i - 2];
            cache[i] = term1 + term2;
        }
    }
}

/* Return the nth term of OEIS A244430. */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
