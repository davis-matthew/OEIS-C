/*
 * A367794  G.f. A(x) satisfies A(x) = 1 / (1 - x * A(x^4)).
 * https://oeis.org/A367794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 71, u64 n <= 140,
 *            u128 n <= 277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A367794: a(n) = Sum_{k=0..⌊(n-1)/4⌋} a(k) * a(n-1-4*k);
    a(0) = 1
    a(1) = 1
    a(2) = 1
    a(3) = 1
    a(4) = 1   ... */

#include <stdlib.h>   /* for malloc, realloc, abort */

static seq_t *cache = NULL;      /* array of computed values              */
static size_t cache_cap = 0;     /* allocated capacity of the array       */
static size_t computed = 0;      /* number of entries in cache that are valid */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap > (size_t)n + 1) return;          /* already large enough */

    size_t new_cap = (size_t)n + 1;                 /* we need indices 0 .. n */
    if (new_cap < cache_cap)                       /* check overflow */
        new_cap = cache_cap ? cache_cap * 2 : 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                             /* fatal error */

    cache       = tmp;
    cache_cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialize cache if necessary */
    if (!cache) {
        ensure_cache_cap(0);                    /* capacity at least 1 */
        cache[0] = (seq_t)1;                    /* a(0) = 1 */
        computed = 1;
    }

    /* For n == 0 we already have the value */
    if (n == 0) return cache[0];

    /* Make sure the cache can hold all indices up to n */
    ensure_cache_cap(n);
    size_t u_n = (size_t)n;                     /* for indexing */

    /* Compute missing entries up to n */
    for (size_t i = computed; i <= u_n; ++i) {
        seq_t total = (seq_t)0;
        size_t maxk = (i - 1) / 4;
        for (size_t k = 0; k <= maxk; ++k) {
            size_t j = i - 1 - 4 * k;
            seq_t prod = cache[k] * cache[j];
            total += prod;
        }
        cache[i] = total;
    }
    computed = u_n + 1;
    return cache[u_n];
}
