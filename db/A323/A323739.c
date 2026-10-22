/*
 * A323739  a(n) is the number of residues modulo (4*primorial(n)) of the
 *          squares of primes greater than or equal to prime(n+1).
 * https://oeis.org/A323739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ∏_{k=1..n} (prime(k)-1) / 2 Recurrence: a(0) = 2; for
 *            n≥1: a(n) = a(n-1) * (prime(n)-1) / 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

/* Compute the n-th term of A323739 */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;   /* last computed index (>=0) */

    /* Initialise cache for a(0) on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* unlikely */
        cache[0] = (seq_t)2;                  /* a(0) = 2 */
        cache_last = 0;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t p = (seq_t)get_prime(i);           /* prime(i) */
        seq_t prod = cache[i - 1] * (p - (seq_t)1);
        cache[i] = prod / (seq_t)2;              /* exact division */
    }
    cache_last = n;

    return cache[n];
}
