/*
 * A292621  a(n) = a(n-1) + a(floor(log(n))) with a(1) = 1, a(2) = 2.
 * https://oeis.org/A292621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(floor(log(n))), a(1)=1, a(2)=2
 * Range:     u8 n <= 84, u16 n <= 8129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_logn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/floor_logn.h"

/* Static cache for computed values.
   cache[i] = a(i), indices start at 1.
   cache[0] is unused but reserved for simplicity. */
static seq_t *cache = NULL;     /* pointer to cache array */
static size_t  cache_len = 0;   /* allocated length (≥ n+1) */
static idx_t   last_computed = 0; /* highest n already computed */

/* Ensure the cache is large enough to store indices up to n.
   Allocate or reallocate as needed. */
static void ensure_cache_size(idx_t n)
{
    if ((idx_t)cache_len > n)
        return;
    size_t new_len = (size_t)n + 1;   /* include index 0 */
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();               /* fatal error if allocation fails */
    cache = tmp;
    cache_len = new_len;
}

/* Initialisation of base cases. */
static void init_cache(void)
{
    if (cache)
        return;                    /* already initialised */
    /* Allocate space for indices 0..2. */
    cache = (seq_t*)malloc(3 * sizeof(seq_t));
    if (!cache) abort();
    cache[0] = (seq_t)0;           /* unused */
    cache[1] = (seq_t)1;           /* a(1) */
    cache[2] = (seq_t)2;           /* a(2) */
    cache_len     = 3;
    last_computed = 2;
}

/* Sequence entry a(n).  Memoises values for any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not used, safety */
    init_cache();                 /* set up base values */
    if (n <= last_computed)
        return cache[n];

    /* Ensure cache can hold up to n */
    ensure_cache_size(n);

    /* Compute successive values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        idx_t log_i = floor_logn(i); /* log base e, floor */
        /* log_i is never 0 for i >= 3, and base cases cover i=1,2 */
        cache[i] = cache[i - 1] + cache[log_i];
    }
    last_computed = n;
    return cache[n];
}
