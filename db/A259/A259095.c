/*
 * A259095  Triangle read by rows: T(n,r) = number of arrangements of n
 *          pennies in rows, with r contiguous pennies in the bottom row, and
 *          each higher row consisting of contiguous pennies, each touching
 *          two pennies in the row below (1 <= r <= n).
 * https://oeis.org/A259095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col), where T(n,r) is the sum over partitions of n
 *            into distinct parts with maximum part r of the product of
 *            successive differences between parts. The recurrence used is:
 *            T(n,r) = 1 if n == r T(n,r) = Σ_{t=1}^{r-1, n-r≥t} (r-t) *
 *            T(n-r, t) otherwise
 * Range:     u8 n <= 287, u16 n <= 1450, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t **dp = NULL;   /* dp[i][j] holds T(i,j) for 1 <= j <= i */
static idx_t dpRows = 0;

/* Find the row number r such that r*(r+1)/2 >= idx.  idx is 1‑based. */
static inline idx_t row_from_index(idx_t idx)
{
    if (idx == 0) return 0;
    long double val = sqrtl((long double)8 * (long double)idx + 1.0L);
    idx_t r = (idx_t)((val - 1) / 2);
    while (r * (r + 1) / 2 < idx) ++r;
    return r;
}

/* Ensure that entries dp[0..target] are computed.  Dynamically grow
   the table as required. */
static void ensure_rows(idx_t target)
{
    if (dpRows >= target) return;

    dp = (seq_t **)realloc(dp, (target + 1) * sizeof(seq_t *));
    for (idx_t i = dpRows + 1; i <= target; ++i) {
        dp[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));  /* indices 0..i */
        for (idx_t r = 1; r <= i; ++r) {
            if (r == i) {
                dp[i][r] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            idx_t remaining = i - r;
            for (idx_t t = 1; t < r; ++t) {
                if (remaining < t) continue;
                seq_t prod = dp[remaining][t];
                if (!prod) continue;
                sum += (seq_t)(r - t) * prod;
            }
            dp[i][r] = sum;
        }
    }
    dpRows = target;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t row = row_from_index(n);
    idx_t col = n - (row - 1) * row / 2;
    ensure_rows(row);
    return dp[row][col];
}
