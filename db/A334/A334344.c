/*
 * A334344  Binary Moran numbers: numbers k such that k divided by its binary
 *          weight (A000120) is a prime number.
 * https://oeis.org/A334344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where k is the n‑th integer such that k divided by its
 *            binary weight (popcount(k)) is a prime number
 * Range:     u8 n <= 17, u16 n <= 1093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

/* Efficient popcount for seq_t by repeated x &= x-1. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x)
    {
        x &= (x - 1);
        ++c;
    }
    return c;
}

/* Cached sequence values and state variables. */
static seq_t *cache      = NULL;  /* array of found terms   */
static size_t cache_size = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity     */
static seq_t last_checked = 0;    /* last number examined   */

/* Return the n-th term of OEIS A334344 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but guard against bad calls */

    /* Return cached value if already known. */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Ensure enough capacity for new elements. */
    if (cache_size == cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        if (cache)
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Continue searching until we have n terms. */
    while (cache_size < (size_t)n)
    {
        /* Move to next candidate integer. */
        last_checked = last_checked + (seq_t)1;

        /* Compute binary weight (popcount). */
        unsigned w = popcount_seq_t(last_checked);
        if (w == 0)
            continue; /* safety, should never happen for positive numbers */

        /* Check divisibility and primality of quotient. */
        if ((last_checked % (seq_t)w) == (seq_t)0)
        {
            seq_t q = last_checked / (seq_t)w;
            if (isPrimeGeneric(q))
            {
                cache[cache_size++] = last_checked;
                /* Ensure capacity for next insertion. */
                if (cache_size == cache_cap)
                {
                    size_t new_cap = cache_cap * 2;
                    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                    if (!new_cache) abort();
                    memcpy(new_cache, cache, cache_size * sizeof(seq_t));
                    free(cache);
                    cache = new_cache;
                    cache_cap = new_cap;
                }
            }
        }
    }

    return cache[(size_t)n - 1];
}
