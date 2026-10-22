/*
 * A337187  a(n) = 1 + Sum_{k=0..n-2} binomial(n-2,k) * a(k) * a(n-k-2).
 * https://oeis.org/A337187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=0..n-2} binomial(n-2,k) * a(k) * a(n-k-2) with
 *            a(0)=1, a(1)=1. Computed using dynamic programming and
 *            memoization. The table is grown on demand so that calls in any
 *            order are fast
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types provided by the build.  For C++ builds
   seq_t may be an arbitrary-precision class; the operations
   (+, -, *, /, ==, ...) are assumed to be available. */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;   /* array a[0..size-1] */
    static idx_t cache_size = 0;  /* next free index */

    /* Grow and compute values up to n if necessary */
    if (n >= cache_size) {
        /* Allocate new array of size n+1 */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);  /* allocation failed */
        /* Initialize newly allocated entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                new_cache[i] = (seq_t)1;
            } else {
                /* Compute a(i) by the recurrence */
                seq_t sum = (seq_t)1;      /* the leading constant 1 */

                /* binomial(i-2,0) = 1 */
                seq_t binom = (seq_t)1;
                idx_t m = i - 2;          /* the upper index in binomial */

                for (idx_t k = 0; k <= m; ++k) {
                    /* term = binom * a(k) * a(m-k) */
                    seq_t prod = new_cache[k] * new_cache[m - k];
                    prod = prod * binom;
                    sum += prod;

                    /* Update binomial coefficient for next k,
                       if not at the end. */
                    if (k < m) {
                        /* binom = binom * (m - k) / (k + 1) */
                        seq_t numer = (seq_t)(m - k);
                        seq_t denom = (seq_t)(k + 1);
                        binom = (binom * numer) / denom;
                    }
                }
                new_cache[i] = sum;
            }
        }
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
