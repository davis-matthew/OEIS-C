/*
 * A373070  Number of partitions of n such that (smallest part) >= 5*(number
 *          of parts).
 * https://oeis.org/A373070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} p_k(n-5·k²), where p_k(m) is the number of
 *            partitions of m into parts ≤ k
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* WARNING: the build system must provide typedefs for idx_t and seq_t.  */
/* idx_t  : unsigned integer type large enough to hold the index (e.g. uint64_t).  */
/* seq_t  : unsigned integer type or arbitrary‑precision integer class providing +, -, *, /, % and comparison operators.  */

#include <stddef.h>
#include <stdlib.h>

/* Integer square root: floor(sqrt(n)) (pure integer, no floats).  */
static inline idx_t int_sqrt(idx_t n) {
    idx_t r = 0;
    idx_t bit = (idx_t)1 << ((sizeof(idx_t) * 8 - 2) & ~1); /* highest power of 4 ≤ n */
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Global DP table for p_k(m): dp[m][k] stored row major.  */
static seq_t *dp = NULL;
static idx_t maxN_global = 0;   /* largest n for which dp is valid */
static idx_t maxK_global = 0;   /* maximum part size (k) stored in dp */

/* Recompute the DP table for all m ≤ newMaxN and k ≤ newMaxK.  */
static void ensure_dp(idx_t newMaxN) {
    if (dp != NULL && newMaxN <= maxN_global) return;  /* already large enough */

    idx_t newMaxK = int_sqrt(newMaxN / 5);          /* ceil is safe, no harm */
    size_t rows = (size_t)newMaxN + 1;
    size_t cols = (size_t)newMaxK + 1;
    seq_t *newdp = (seq_t *)malloc(rows * cols * sizeof(seq_t));
    if (newdp == NULL) return;                      /* allocation failed; caller will get 0 */

    /* Base case: dp[0][k] = 1 for all k, dp[m>0][0] = 0 */
    for (idx_t k = 0; k <= newMaxK; ++k) {
        newdp[k] = 1;
    }

    for (idx_t m = 1; m <= newMaxN; ++m) {
        newdp[(size_t)m * cols] = 0;               /* dp[m][0] = 0 */
        for (idx_t k = 1; k <= newMaxK; ++k) {
            seq_t val = newdp[(size_t)m * cols + (k - 1)];   /* dp[m][k-1] */
            if (m >= k) {
                val += newdp[(size_t)(m - k) * cols + k];    /* dp[m-k][k] */
            }
            newdp[(size_t)m * cols + k] = val;
        }
    }

    free(dp);
    dp = newdp;
    maxN_global = newMaxN;
    maxK_global = newMaxK;
}

/* Main sequence function */
seq_t a(idx_t n) {
    ensure_dp(n);

    idx_t kmax = int_sqrt(n / 5);
    seq_t result = 0;
    size_t cols = (size_t)maxK_global + 1;

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - 5 * k * k;
        result += dp[(size_t)m * cols + k];
    }

    return result;
}
