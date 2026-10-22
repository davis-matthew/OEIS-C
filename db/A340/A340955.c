/*
 * A340955  Number of ways to write n as an ordered sum of 10 nonzero
 *          triangular numbers.
 * https://oeis.org/A340955
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered decompositions of n into 10 non‑zero
 *            triangular numbers (n ≥ 10). The values are computed by a
 *            dynamic‑programming recurrence
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 400, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/seq_free.h"

static seq_t *dp = NULL;        /* DP table: 11 rows × (current_max+1) columns */
static size_t stride = 0;       /* number of columns in dp */
static idx_t current_max = 0;   /* largest n that dp has been computed for */

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 10)                /* below the offset: value is irrelevant */
        return 0;

    /* Re‑compute the table if we need values for a larger n. */
    if (current_max < n) {
        size_t new_stride = (size_t)n + 1;
        seq_t *new_dp = alloc_seq(11 * new_stride);   /* constructs seq_t elements */

        if (!new_dp)                /* allocation failure */
            return 0;

        /* Free old table if present */
        if (dp)
            seq_free(dp);

        dp = new_dp;
        stride = new_stride;
        current_max = n;

        /* Clear the table */
        for (size_t i = 0; i < 11 * stride; ++i)
            dp[i] = 0;

        /* Triangular numbers up to n (stored as idx_t to avoid overflow). */
        idx_t tri[200];      /* 200 suffices for n ≤ 10000 */
        size_t tri_cnt = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t t = (idx_t)(((seq_t)k * (k + 1)) / 2);
            if (t > n)
                break;
            tri[tri_cnt++] = t;
        }

        /* DP base case: 0 summands, sum 0 */
        dp[0] = 1;

        /* Build rows for 1 … 10 summands */
        for (int k = 1; k <= 10; ++k) {
            size_t row  = (size_t)k * stride;
            size_t prev = (size_t)(k - 1) * stride;

            for (size_t ti = 0; ti < tri_cnt; ++ti) {
                idx_t t = tri[ti];
                for (idx_t s = t; s <= n; ++s) {
                    dp[row + (size_t)s] += dp[prev + (size_t)(s - t)];
                }
            }
        }
    }

    return dp[10 * stride + (size_t)n];
}
