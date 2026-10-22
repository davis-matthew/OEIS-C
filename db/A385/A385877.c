/*
 * A385877  a(n) = 1 + Sum_{k=0..n-1} binomial(k+4,5) * a(k) * a(n-1-k).
 * https://oeis.org/A385877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} binomial(k+4,5) * a(k) * a(n-1-k) a(0)
 *            = 1 (All other terms are defined by the recurrence)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming and memoization.
   It works for any integer type seq_t and idx_t defined by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoized values */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* current allocated size of cache   */
    static idx_t computed = 0;    /* number of entries computed (next
                                    index to compute)                 */

    /* Ensure the cache is large enough */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise base case if this is the first call */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {          /* base case already set */
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        uint64_t prev_ck = 0;  /* coefficient for k-1 (used to compute next) */

        for (idx_t k = 0; k < i; ++k) {
            uint64_t ck;
            if (k == 0) {
                ck = 0;                     /* binomial(4,5) = 0          */
            } else if (k == 1) {
                ck = 1;                     /* binomial(5,5) = 1          */
            } else {
                /* Compute binomial(k+4,5) from binomial(k+3,5):
                   C(k+4,5) = C(k+3,5) * (k+4) / (k-1)  (k >= 2)   */
                unsigned __int128 tmp = (unsigned __int128)prev_ck * (k + 4);
                ck = (uint64_t)(tmp / (k - 1));
            }
            prev_ck = ck;

            /* Multiply a(k), a(i-1-k) and the coefficient ck.
               All operations are on seq_t and are permitted by the
               specification. */
            seq_t prod = cache[k] * cache[i - 1 - k] * (seq_t)ck;
            sum += prod;
        }
        cache[i] = 1 + sum;
    }

    computed = n + 1;
    return cache[n];
}
