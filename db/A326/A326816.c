/*
 * A326816  a(0) = 0, a(1) = 1, and for n > 1, a(n) = Sum_{k = 0..n} a((n-k)
 *          AND k) (where AND denotes the bitwise AND operator).
 * https://oeis.org/A326816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(0)=0, a(1)=1, for n>1 a(n) = Σ_{k=0..n} a((n-k) AND k)
 *            (bitwise AND)
 * Range:     u8 n <= 35, u16 n <= 161, u32 n <= 1347, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array holding computed a(i) */
    static size_t   cap    = 0;     /* allocated capacity of cache */
    static idx_t    next   = 2;     /* next index to compute */

    /* Initialise cache if required */
    if (!cache) {
        cap   = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
    }

    /* Grow cache if ask for larger n */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values up to n */
    for (idx_t i = next; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            /* (i-k) & k fits in idx_t; result is always <= i */
            idx_t idx = (i - k) & k;
            sum += cache[idx];
        }
        cache[i] = sum;
    }
    next = n + 1;

    return cache[n];
}
