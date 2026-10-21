/*
 * A274845  a(0)=1, a(1)=0, a(4n+2) = a(4n+3) = a(4n+5) = (4^(n+1) +(-1)^n)/5,
 *          a(4n+4) = (2*4^(n+1) -3*(-1)^n)/5.
 * https://oeis.org/A274845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 67, u64 n <= 131,
 *            u128 n <= 259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=3, a(7)=3;
   For n ≥ 8:  a(n) = 3*a(n-4) + 4*a(n-8). */
/* Note: All arithmetic is performed in the built-in seq_t type. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, NULL */

static seq_t *cache   = NULL;
static size_t  cap    = 0;
static idx_t   maxidx = (idx_t)-1;   /* largest index computed (-1 for none) */

seq_t a(idx_t n)
{
    /* Lazily allocate cache with the first 8 known terms. */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();   /* fatal failure */

        /* Initialised terms that fit the recurrence for n < 8. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)3;
        cache[7] = (seq_t)3;
        maxidx = 7;
    }

    /* If the requested index is already computed, return it. */
    if (n <= maxidx)
        return cache[(size_t)n];

    /* Ensure the cache has enough space for the new terms. */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* fatal failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = maxidx + 1; i <= n; ++i) {
        seq_t v1 = cache[(size_t)(i - 4)];
        seq_t v2 = cache[(size_t)(i - 8)];
        cache[(size_t)i] = (seq_t)(3 * v1 + 4 * v2);
    }

    maxidx = n;
    return cache[(size_t)n];
}
