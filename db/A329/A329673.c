/*
 * A329673  Number of meanders of length n with Motzkin-steps avoiding the
 *          consecutive steps HH.
 * https://oeis.org/A329673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = (2*a(n-1) + 4*n*a(n-2) + (8*n - 16)*a(n-3) + (4*n -
 *            12)*a(n-4))/(n+1) Initial values: a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=10 Recurrence holds for n >= 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp      = NULL;
    static size_t capacity = 0;
    static size_t computed = 0;      /* number of elements valid in dp (0..computed-1) */

    /* Ensure allocation */
    if ((size_t)n + 1 > capacity) {
        size_t newcap = capacity == 0 ? 4 : capacity;
        if (newcap <= n) {
            newcap = (n + 1) * 2;
        }
        seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
        dp = tmp;
        capacity = newcap;
    }

    /* Initialize base values lazily */
    if (computed == 0) {
        computed = 1;
        dp[0] = (seq_t)1;
        if (capacity >= 2) dp[1] = (seq_t)2;
        if (capacity >= 3) dp[2] = (seq_t)4;
        if (capacity >= 4) dp[3] = (seq_t)10;
    }

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)computed; i <= n; ++i) {
        if (i == 0) {
            dp[0] = (seq_t)1; continue;
        }
        if (i == 1) {
            dp[1] = (seq_t)2; continue;
        }
        if (i == 2) {
            dp[2] = (seq_t)4; continue;
        }
        if (i == 3) {
            dp[3] = (seq_t)10; continue;
        }

        /* Recurrence for i >= 4 */
        seq_t term1 = (seq_t)2 * dp[i - 1];
        seq_t term2 = (seq_t)4 * (seq_t)i * dp[i - 2];
        seq_t coeff3 = (seq_t)((i * 8u) - 16u);
        seq_t coeff4 = (seq_t)((i * 4u) - 12u);
        seq_t term3 = coeff3 * dp[i - 3];
        seq_t term4 = coeff4 * dp[i - 4];

        seq_t numer = term1 + term2 + term3 + term4;
        dp[i] = numer / ((seq_t)(i + 1));
    }

    computed = (size_t)n + 1;
    return dp[n];
}
