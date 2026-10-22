/*
 * A331519  a(0) = 1; a(n) = Sum_{k=1..n} (binomial(n,k) mod 2) * a(k-1) *
 *          a(n-k).
 * https://oeis.org/A331519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{k=1..n} (binomial(n,k) mod 2) * a(k-1) * a(n-k),  n ≥ 1 */

#include <stdlib.h>
#include <stdbool.h>

/* cache for already computed values */
static seq_t *cache = NULL;  /* pointer to an array of values */
static idx_t  cache_cap = 0; /* current capacity of the cache */

/* ensure that the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;                   /* we already have enough space */

    idx_t new_cap = n + 1;
    /* allocate a new array (C-style cast required for C++ compilation) */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache)
        abort();                   /* out of memory */

    /* copy old values, if any, and initialize the rest to zero */
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* base case */
    if (n == 0) {
        cache[0] = (seq_t)1;
        return (seq_t)1;
    }

    /* return cached result if it already exists */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute a(n) using the recurrence */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        /* binomial(n,k) is odd iff (k & (n-k)) == 0 (Lucas' theorem) */
        if ((k & (n - k)) == 0) {
            seq_t left  = a(k - 1); /* a(k-1) */
            seq_t right = a(n - k); /* a(n-k) */
            sum += left * right;
        }
    }

    cache[n] = sum;
    return sum;
}
