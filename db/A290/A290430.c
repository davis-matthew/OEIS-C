/*
 * A290430  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of (Sum_{j>=0} x^(j*(j+1)*(2*j+1)/6))^k.
 * https://oeis.org/A290430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of ways to write n as a sum of k square pyramidal
 *            * numbers, where the pair (sum,k) is obtained by flattening the
 *            square array * of A(n,k) by antidiagonals (OEIS A290430)
 * Range:     u8 n <= 123, u16 n <= 384, u32 n <= 1239, u64 n <= 4310,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>


/* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
static inline idx_t diagonal_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* increase hi until hi*(hi+1)/2 > n */
    while (( (__uint128_t)hi * (hi + 1) ) / 2 <= n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (( (__uint128_t)mid * (mid + 1) ) / 2 <= n) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Map flattened index n to coordinates (sum,k). */
    idx_t d   = diagonal_of(n);
    idx_t off = ( (__uint128_t)d * (d + 1) ) / 2;   /* elements before this diagonal */
    idx_t t   = n - off;                            /* position inside diagonal */
    idx_t sum = t;                                  /* first coordinate (row) */
    idx_t k   = d - t;                              /* second coordinate (column) */

    /* Special case: n = 0 => (0,0). */
    if (sum == 0 && k == 0) return (seq_t)1;

    /* Allocate DP table of size (sum+1) * (k+1). */
    size_t N = (size_t)sum;
    size_t K = (size_t)k;
    size_t total = (N + 1) * (K + 1);

    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp) return (seq_t)0; /* allocation failure */

    /* Base: dp[0][0] = 1; all other entries start at 0. */
    dp[0] = (seq_t)1;

    /* Generate list of square pyramidal numbers <= sum. */
    idx_t *pyr = NULL;
    size_t pyr_cap = 0, pyr_sz = 0;
    for (idx_t j = 0;; ++j) {
        __uint128_t prod = (__uint128_t)j * (j + 1) * (2 * j + 1);
        idx_t val = (idx_t)(prod / 6);
        if (val > sum) break;
        if (pyr_sz == pyr_cap) {
            pyr_cap = pyr_cap ? pyr_cap * 2 : 8;
            pyr = (idx_t *)realloc(pyr, pyr_cap * sizeof(idx_t));
            if (!pyr) { free(dp); return (seq_t)0; }
        }
        pyr[pyr_sz++] = val;
    }

    /* DP: number of k-tuples summing to each value. */
    for (size_t m = 1; m <= K; ++m) {
        for (size_t p = 0; p < pyr_sz; ++p) {
            idx_t pv = pyr[p];
            for (size_t s = pv; s <= N; ++s) {
                dp[s * (K + 1) + m] +=
                    dp[(s - pv) * (K + 1) + (m - 1)];
            }
        }
    }

    seq_t result = dp[N * (K + 1) + K];

    free(dp);
    free(pyr);
    return result;
}
