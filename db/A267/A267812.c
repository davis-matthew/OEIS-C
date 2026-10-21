/*
 * A267812  Decimal representation of the n-th iteration of the "Rule 217"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n‑1) – 14*a(n‑2) + 8*a(n‑3) for n>4 with a(0)=1,
 *            a(1)=1, a(2)=27, a(3)=119, a(4)=495
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers needed for memory handling. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* Allocation helpers that work in both C and C++. */
#ifdef __cplusplus

#else

#endif

/* Static cache to store already computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* total capacity of the cache array */
static size_t cache_len = 0;   /* number of valid entries in the cache */

/* Make sure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (cache_cap >= required)
        return;                     /* already large enough */

    /* Grow the cache (doubling strategy) to avoid repeated reallocs. */
    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap < required)
        new_cap *= 2;

    seq_t *new_cache = alloc_seq(new_cap);
    if (cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free_seq(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Return the nth term of the sequence, defined for all n ≥ 0. */
seq_t a(idx_t n)
{
    /* First call: initialise the cache with the first five terms. */
    if (cache == NULL) {
        ensure_cache(4);              /* need indices 0…4 */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 27;
        cache[3] = 119;
        cache[4] = 495;
        cache_len = 5;
    }

    /* If the requested term is already cached, return it directly. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Make sure the cache can accommodate the new terms. */
    ensure_cache(n);

    /* Compute all missing terms up to and including n. */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        /* Use the recurrence relation with seq_t arithmetic. */
        seq_t t1 = cache[k - 1] * 7;   /* 7*a(k-1) */
        seq_t t2 = cache[k - 2] * 14;  /* 14*a(k-2) */
        seq_t t3 = cache[k - 3] * 8;   /* 8*a(k-3) */

        /* Compute t1 - t2 + t3. All intermediate results are
           guaranteed to be non‑negative for n ≥ 5. */
        seq_t temp = t1 + t3;   /* avoid temporary negative values */
        cache[k] = temp - t2;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
