/*
 * A258948  a(1)=1, a(2)=2; for n>2, a(n) = (1/2)*a(n-1)*a(n-2) + a(n-1) +
 *          a(n-2).
 * https://oeis.org/A258948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2; for n>2, a(n) = (1/2)*a(n-1)*a(n-2) + a(n-1)
 *            + a(n-2)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence alternative: a(n)+2 = (1/2)*(a(n-1)+2)*(a(n-2)+2) */

#include <stdlib.h>

/* Cached values: index i stores a(i+1)   (0‑based) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Allocate the initial cache if needed */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;                        /* a(1) */
        cache[1] = (seq_t)2;                        /* a(2) */
        cache_len = 2;
    }

    /* If we already have the value, return it */
    if (n <= cache_len) {
        return cache[n-1];
    }

    /* Extend the cache to hold up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) return (seq_t)0; /* allocation failure */
    cache = new_cache;

    /* Compute missing values iteratively */
    for (idx_t i = cache_len + 1; i <= n; ++i) {          /* i is 1‑based index */
        seq_t a_prev  = cache[i-2];                       /* a(i-1) */
        seq_t a_prev2 = cache[i-3];                       /* a(i-2) */

        /* Use the stable recurrence a(n)+2 = ((a(n-1)+2)*(a(n-2)+2))/2 */
        seq_t p1 = a_prev + (seq_t)2;
        seq_t p2 = a_prev2 + (seq_t)2;
        seq_t product;

        if ((p1 & (seq_t)1) == 0) {  /* p1 is even */
            product = (p1 >> 1) * p2;
        } else {                     /* p2 must be even */
            product = p1 * (p2 >> 1);
        }

        cache[i-1] = product - (seq_t)2;   /* Store a(i) */
    }

    cache_len = n;

    return cache[n-1];
}
