/*
 * A275428  Number of set partitions of [n] such that ten is a multiple of
 *          each block size.
 * https://oeis.org/A275428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * Recurrence: a(n) = Σ_{k∈{1,2,5,10}, k≤n} binom(n-1,
 *            k-1) * a(n-k) * (The egf is exp(x + x^2/2 + x^5/5! + x^10/10!) )
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes binomial coefficient for k ≤ 9 (i.e. nCk) */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* array of already computed values */
    static idx_t dp_cap = 0;     /* allocated capacity */
    static idx_t dp_size = 0;    /* number of entries computed (dp_size = max_index+1) */

    /* initialise on first call */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (dp == NULL) return 0;     /* allocation failure propagates a zero value */
        dp[0] = 1;                    /* a(0) = 1 */
        dp_cap = 1;
        dp_size = 1;
    }

    /* grow the array if needed */
    if (n >= dp_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;    /* allocation failure */
        dp = tmp;
        dp_cap = (idx_t)new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = dp_size; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 1) {                        /* block size 1 */
            val += dp[i-1];                  /* binom(i-1,0) = 1 */
        }
        if (i >= 2) {                        /* block size 2 */
            seq_t bin = (seq_t)(i-1);        /* binom(i-1,1) */
            val += dp[i-2] * bin;
        }
        if (i >= 5) {                        /* block size 5 */
            seq_t bin = binom_small(i-1, 4); /* binom(i-1,4) */
            val += dp[i-5] * bin;
        }
        if (i >= 10) {                       /* block size 10 */
            seq_t bin = binom_small(i-1, 9); /* binom(i-1,9) */
            val += dp[i-10] * bin;
        }
        dp[i] = val;
    }

    dp_size = n + 1;
    return dp[n];
}
