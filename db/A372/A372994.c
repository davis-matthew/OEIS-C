/*
 * A372994  a(n) = n! a(n-1) / a(n-2), where a(0) = 1, a(1) = 2.
 * https://oeis.org/A372994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * a(n-1) / a(n-2); Initial values: a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for GCD needed for exact cancellation. */
#include "../utils/gcd.h"

#include <stdlib.h>
#include <stddef.h>

/* Cache of previously computed values. */
static seq_t *cache   = NULL;   /* array of a(0) .. a(cache_len-1) */
static size_t cache_len = 0;    /* number of cached elements */
static seq_t fact = 1;          /* factorial of (cache_len-1): (cache_len-1)! */

/* Ensure that the cache has at least (n+1) slots. */
static void ensure_cache(idx_t n)
{
    if (cache_len > (size_t)n)
        return; /* already large enough */

    size_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE); /* allocation failure */

    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)2;  /* a(1) */
        cache_len = 2;
        fact = (seq_t)1;      /* 1! */
    }

    /* Determine where to start computing new values. */
    size_t start = cache_len;          /* index of first not yet computed value */
    ensure_cache(n);                   /* ensure room for values up to a(n) */

    /* Compute missing terms using exact cancellation with GCD. */
    for (idx_t i = start; i <= n; ++i) {
        /* Update factorial: fact = i! */
        fact = fact * (seq_t)i;

        /* Compute a(i) = fact * a(i-1) / a(i-2) with cancellation. */
        seq_t g1 = gcd(fact, cache[i-2]);
        seq_t fact_red = fact / g1;
        seq_t denom = cache[i-2] / g1;

        seq_t g2 = gcd(cache[i-1], denom);
        seq_t a1_red = cache[i-1] / g2;
        denom /= g2;

        seq_t g3 = gcd(fact_red, denom);
        fact_red /= g3;
        denom /= g3;            /* denom should now be 1 */

        cache[i] = fact_red * a1_red;
    }

    return cache[n];
}
