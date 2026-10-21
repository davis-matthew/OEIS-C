/*
 * A278899  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 107", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-2) - 10100*a(n-6) + 10000*a(n-8) for n > 15
 *            Initial values: a(0) = 1 a(1) = 10 a(2) = 100 a(3) = 1111 a(4) =
 *            100 a(5) = 111011 a(6) = 0 a(7) = 11101111 a(8) = 1010000 a(9) =
 *            1110101111 a(10) = 101000000 a(11) = 111011111111 a(12) =
 *            10101000000 a(13) = 11101010111111 a(14) = 1010100...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* memoisation array */
static size_t cache_len  = 0;      /* number of entries currently stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(size_t n)
{
    if (cache_cap <= n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();              /* allocation failure */
        cache      = new_cache;
        cache_cap  = new_cap;
    }
}

/* First 16 terms of the sequence, stored as constants. */
static const seq_t init_vals[16] = {
    /* a(0) */              1ULL,
    /* a(1) */             10ULL,
    /* a(2) */            100ULL,
    /* a(3) */           1111ULL,
    /* a(4) */             100ULL,
    /* a(5) */          111011ULL,
    /* a(6) */              0ULL,
    /* a(7) */        11101111ULL,
    /* a(8) */       1010000ULL,
    /* a(9) */     1110101111ULL,
    /* a(10) */    101000000ULL,
    /* a(11) */  111011111111ULL,
    /* a(12) */ 10101000000ULL,
    /* a(13) */11101010111111ULL,
    /* a(14) */1010100000000ULL,
    /* a(15) */1110101111111111ULL
};

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Small indices are returned directly. */
    if (n < 16)
        return init_vals[(size_t)n];

    size_t idx = (size_t)n;

    /* Allocate and initialise cache if first used. */
    if (cache == NULL)
        ensure_capacity(16);
    if (cache_len < 16)
    {
        /* Copy the first 16 terms into the cache. */
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        cache_len = 16;
    }

    /* Grow cache if necessary. */
    if (idx >= cache_cap)
        ensure_capacity(idx);

    /* Compute missing values up to idx. */
    for (size_t i = cache_len; i <= idx; ++i)
    {
        seq_t term;
        /* Use recurrence for n >= 16. */
        term = cache[i-2] * 101                 /* 101*a(n-2) */
             - cache[i-6] * 10100               /* 10100*a(n-6) */
             + cache[i-8] * 10000;              /* 10000*a(n-8) */
        cache[i] = term;
    }

    cache_len = idx + 1;
    return cache[idx];
}
