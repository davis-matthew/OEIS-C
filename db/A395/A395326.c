/*
 * A395326  Triangle read by rows: T(n,k) is the number of partitions of
 *          {1..2n} that are invariant under a permutation consisting of n
 *          2-cycles with k orbits of blocks.
 * https://oeis.org/A395326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) equals the triangle T(m,k) where m = max integer with
 *            m*(m+1)/2 <= n, k = n - m*(m+1)/2. T(m,k) counts partitions of
 *            {1..2m} that are invariant under a permutation consisting of m
 *            disjoint 2‑cycles with k orbits of blocks. The values are given
 *            by the weighted Stirling numbers of the second k...
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 95, u64 n <= 258,
 *            u128 n <= 676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stddef.h>

/* Adjust the following constant if the sequence may be requested for
   n larger than 10000.  With MAX_M = 200 we can handle n up to about
   200*201/2 = 20100 safely.  */
#define MAX_M 200

static bool initialized = false;
static seq_t dp[MAX_M + 1][MAX_M + 1];
static seq_t w[MAX_M + 1];
static seq_t binom_tbl[MAX_M + 1][MAX_M + 1];

static void init_tables(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Precompute binomial coefficients binom_tbl[n][k] = C(n,k). */
    for (idx_t n = 0; n <= MAX_M; ++n)
    {
        binom_tbl[n][0] = 1;
        for (idx_t k = 1; k <= n; ++k)
        {
            binom_tbl[n][k] = binom_tbl[n-1][k-1] + binom_tbl[n-1][k];
        }
    }

    /* Precompute weights w[r] = 1 + 2^(r-1). */
    w[0] = 0; /* unused */
    for (idx_t r = 1; r <= MAX_M; ++r)
    {
        seq_t pow2 = ((seq_t)1 << (r - 1)); /* 2^(r-1) */
        w[r] = pow2 + 1;
    }

    /* DP for weighted Stirling numbers (Bell polynomials). */
    dp[0][0] = 1;
    for (idx_t i = 1; i <= MAX_M; ++i)
    {
        for (idx_t j = 1; j <= i; ++j)
        {
            seq_t sum = 0;
            /* r = size of new part */
            for (idx_t r = 1; r <= i - j + 1; ++r)
            {
                seq_t b = binom_tbl[i - 1][r - 1];
                seq_t term = b * w[r];
                term = term * dp[i - r][j - 1];
                sum += term;
            }
            dp[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;            /* a(0) = 1 */
    init_tables();

    /* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
    double disc = sqrt(8.0L * n + 1.0L);
    idx_t m = (idx_t)((disc - 1.0L) / 2.0L);
    /* Correct possible rounding errors */
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;
    while (m * (m + 1) / 2 > n)
        --m;

    idx_t k = n - m * (m + 1) / 2;

    if (k == 0)            /* For n > 0, T(m,0) = 0 */
        return 0;

    if (m > MAX_M || k > m)
        return 0;           /* Should not happen for n <= 10000 */

    return dp[m][k];
}
