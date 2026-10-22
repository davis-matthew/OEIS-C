/*
 * A307068  Expansion of 1/(1 - Sum_{k>=1} k!*x^(k*(k+1)/2) / Product_{j=1..k}
 *          (1 - x^j)).
 * https://oeis.org/A307068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A307068
    Formula: a(n) = Sum_{k=1..n} A032020(k) * a(n-k),  a(0) = 1
    (A032020(k) = k! * coeff of x^k in 1/∏_{j=1..k}(1−x^j) )
    This implementation computes the coefficients of B(x) = Σ A032020(k)x^k
    using partition DP, then inverts the series to obtain a(n).  The
    algorithm runs in O(n·√n) time and O(n) memory, suitable for n up to
    at least 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Function prototype required by the build system */
seq_t a(idx_t n);

/* Helper to compute floor( (sqrt(1+8*n)-1)/2 ) without floating point */
static idx_t max_k_for_n(idx_t n)
{
    idx_t k = 0;
    while (((k + 1) * (k + 2)) / 2 <= n)
        ++k;
    return k;
}

seq_t a(idx_t n)
{
    /* Static cache:  a_cache[0..cache_sz-1], b_cache[0..cache_sz-1] */
    static idx_t cache_sz = 0;
    static seq_t *a_cache = NULL;
    static seq_t *b_cache = NULL;

    /* Rebuild cache if requested n is beyond current range */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;                     /* array size required */

        /* Allocate new arrays for a and b */
        seq_t *new_a = (seq_t *)calloc((size_t)new_sz, sizeof(seq_t));
        seq_t *new_b = (seq_t *)calloc((size_t)new_sz, sizeof(seq_t));

        /* Partition DP array and factorial array */
        seq_t *dp   = (seq_t *)calloc((size_t)new_sz,  sizeof(seq_t));
        seq_t *fact = (seq_t *)calloc((size_t)(max_k_for_n(n) + 1), sizeof(seq_t));

        /* Base values */
        dp[0]   = 1;
        fact[0] = 1;

        idx_t maxK = max_k_for_n(n);

        /* Compute b(k) = k! * p_k( m ), summed over
           m = n - k(k+1)/2 for each n. */
        for (idx_t k = 1; k <= maxK; ++k) {
            /* factorial(k) */
            fact[k] = fact[k - 1] * (seq_t)k;

            /* Update partitions dp for part size k */
            for (idx_t j = k; j < new_sz; ++j)
                dp[j] += dp[j - k];

            /* offset = k*(k+1)/2 */
            idx_t offset = k * (k + 1) / 2;
            if (offset > n)
                continue;

            /* Add contribution of this k to all relevant b[n] */
            for (idx_t i = offset; i < new_sz; ++i) {
                new_b[i] += fact[k] * dp[i - offset];
            }
        }

        /* Compute a via convolution with b */
        new_a[0] = 1;
        for (idx_t i = 1; i < new_sz; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += new_b[k] * new_a[i - k];
            new_a[i] = sum;
        }

        /* Replace old cache with new data */
        if (a_cache) free(a_cache);
        if (b_cache) free(b_cache);
        a_cache = new_a;
        b_cache = new_b;
        cache_sz = new_sz;

        /* Free temporary arrays */
        free(dp);
        free(fact);
    }

    return a_cache[n];
}
