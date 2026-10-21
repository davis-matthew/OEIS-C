/*
 * A268188  Sum of the sizes of the Durfee squares of all no-leg partitions of
 *          n (or of all no-arm partitions of n).
 * https://oeis.org/A268188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} k * p_k(n-k^2), where p_k(m) is the number of
 *            partitions of m with parts <= k. The generating function is g =
 *            Σ_{k>=1} k*x^(k^2)/Π_{i=1..k}(1-x^i)
 * Range:     u8 n <= 27, u16 n <= 76, u32 n <= 249, u64 n <= 879,
 *            u128 n <= 3272, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Integer square‑root: floor(sqrt(x)) for unsigned 64‑bit values.     */
/* --------------------------------------------------------------------- */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 0xFFFFFFFFUL;   /* sqrt(2^64-1) <= 2^32-1 */
    idx_t best = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid == 0) {
            best = 0;
            lo = 1;
            continue;
        }
        if (mid <= x / mid) {          /* mid*mid <= x without overflow */
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;       /* guard against underflow */
            hi = mid - 1;
        }
    }
    return best;
}

/* --------------------------------------------------------------------- */
/*  Partition table:  dp[k][m] = number of partitions of m with parts <= k
 *  dp is kept alive across calls.                                       */
/* --------------------------------------------------------------------- */
static seq_t **dp = NULL;          /* dp[k][m] */
static idx_t  dp_n = 0;            /* maximum m used in dp rows       */
static idx_t  dp_k = 0;            /* maximum k stored in dp           */

/* Rebuild dp so that it can answer any n up to N. */
static void ensure_table(idx_t N)
{
    idx_t need_n = N;
    idx_t need_k = floor_sqrt_u64(N);

    /* Table already sufficient? */
    if (dp != NULL && dp_n >= need_n && dp_k >= need_k) return;

    /* Free old table */
    if (dp != NULL) {
        for (idx_t i = 0; i <= dp_k; ++i) free(dp[i]);
        free(dp);
    }

    /* Allocate new table */
    dp_n = need_n;
    dp_k = need_k;
    dp = (seq_t **)malloc((dp_k + 1) * sizeof(seq_t *));
    if (!dp) {                    /* allocation failed – give up */
        dp_n = dp_k = 0;
        return;
    }

    /* row 0 : only the empty partition */
    dp[0] = (seq_t *)calloc(dp_n + 1, sizeof(seq_t));
    if (!dp[0]) { free(dp); dp = NULL; dp_n = dp_k = 0; return; }
    dp[0][0] = (seq_t)1;

    /* build rows 1..dp_k */
    for (idx_t k = 1; k <= dp_k; ++k) {
        dp[k] = (seq_t *)calloc(dp_n + 1, sizeof(seq_t));
        if (!dp[k]) { /* allocation failed – abort further rows */ break; }
        seq_t *row = dp[k];
        seq_t *prev = dp[k - 1];
        for (idx_t m = 0; m <= dp_n; ++m) {
            row[m] = prev[m];
            if (m >= k) row[m] += row[m - k];
        }
    }
}

/* --------------------------------------------------------------------- */
/*  The requested sequence function.                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is defined for n>=1 */

    ensure_table(n);               /* build table sufficient for this n */

    idx_t kmax = floor_sqrt_u64(n);
    seq_t sum  = (seq_t)0;

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t m = n - k * k;
        seq_t v = dp[k][m];        /* partitions of m with parts <= k */
        sum += (seq_t)k * v;
    }
    return sum;
}
