/*
 * A281964  Real part of n!*Sum_{k=1..n} i^(k-1)/k, where i is sqrt(-1).
 * https://oeis.org/A281964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A281964
    Real part of n!*∑_{k=1..n} i^(k-1)/k
    Recurrence: a(1)=1,
                a(n+1)=a(n)*(n+1)+n!*cos(Pi*n/2)
    (cos(Pi*n/2) is 1 for n≡0 mod 4, -1 for n≡2 mod 4, 0 otherwise) */
#include <stdlib.h>

/* static cache: cache[i] = a(i)  (1 ≤ i ≤ cache_max) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache array (indices 0..cap‑1) */
static idx_t  cache_max = 0;   /* largest index computed so far   */
static seq_t  fact      = 0;   /* factorial of cache_max : cache_max! */

seq_t a(idx_t n)
{
    /* initialise cache on first invocation */
    if (cache == NULL) {
        cache_cap = 2;                    /* indices 0 and 1 are required */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[1] = (seq_t)1;              /* a(1) = 1 */
        fact     = (seq_t)1;              /* 1!   */
        cache_max = 1;
    }

    /* if already computed, return the cached value */
    if (n <= cache_max) return cache[n];

    /* allocate enough space for indices 0…n */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;           /* want indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* generate a(i+1) for i = cache_max … n‑1 */
    for (idx_t i = cache_max; i < n; ++i) {
        /* term = a(i) * (i+1) + i! * cos(Pi*i/2)  */
        seq_t term = cache[i] * (i + 1);
        idx_t mod4 = i & 3u;   /* i % 4 */
        if (mod4 == 0)         /* cos =  1 */
            term += fact;
        else if (mod4 == 2)    /* cos = -1 */
            term -= fact;     /* values stay non‑negative */

        cache[i + 1] = term;

        /* update factorial for next iteration: fact ← (i+1)! */
        fact *= (seq_t)(i + 1);
    }

    cache_max = n;
    return cache[n];
}
