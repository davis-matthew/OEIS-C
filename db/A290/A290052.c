/*
 * A290052  Number of X-rays of n X n binary matrices with exactly n ones.
 * https://oeis.org/A290052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions of * x_0 + x_1 +
 *            … + x_{2n-2} = n with 0 ≤ x_i ≤ min(i+1, 2n-1-i)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_zero_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the number of distinct X‑rays of n×n binary matrices
 * with exactly n ones.  The counting is performed by a dynamic
 * programming algorithm that enumerates all admissible antidiagonal
 * sum vectors.  It uses only integer arithmetic and works for all
 * build configurations (uint8_t … uint64_t and arbitrary‑precision
 * seq_t). */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_zero_array.h"

static void free_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Main sequence function:  a(idx_t n) */
seq_t a(idx_t n) {
    /* Edge case n == 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Types for convenient loops */
    size_t N = (size_t)n;            /* number of ones */
    size_t m = 2 * N - 1;            /* number of antidiagonals */

    /* Allocate working arrays (N+1 entries) */
    seq_t *dp    = alloc_zero_array(N + 1);
    seq_t *newdp = alloc_zero_array(N + 1);
    seq_t *pref  = alloc_zero_array(N + 1);

    dp[0] = (seq_t)1;                /* base case: sum 0 reachable in 1 way */

    /* Main DP over all antidiagonals */
    for (size_t d = 0; d < m; ++d) {
        size_t cap = (d < N) ? (d + 1) : (size_t)(2 * N - 1 - d); /* capacity */

        /* Build prefix sums of current dp array */
        pref[0] = dp[0];
        for (size_t j = 1; j <= N; ++j)
            pref[j] = pref[j - 1] + dp[j];

        /* Compute newdp using the prefix sums and the capacity constraint */
        for (size_t j = 0; j <= N; ++j) {
            if (j <= cap)
                newdp[j] = pref[j];
            else
                newdp[j] = pref[j] - pref[j - cap - 1];
        }

        /* Swap dp and newdp for the next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[N];            /* number of ways to reach sum N */

    /* Clean up temporary arrays */
    free_array(dp);
    free_array(newdp);
    free_array(pref);

    return result;
}
