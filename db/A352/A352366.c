/*
 * A352366  Triangle read by rows. The incomplete Bell transform of the
 *          Catalan numbers.
 * https://oeis.org/A352366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(s)(m,k) where - m = floor((sqrt(8 n + 1) − 1)/2) - k =
 *            n − m (m + 1)/2 - s(0) = 1, and for i ≥ 1, s(i) =
 *            CatalanNumber(i − 1) - T(s)(0,0) = 1, T(s)(n,0) = 0 for n > 0 -
 *            For n ≥ 1 and k ≥ 1: T(s)(n,k) = Σ_{i=1}^{n} binom(n−1,i−1) ·
 *            s(i) · T(s)(n−i,k−1) The triangle read by rows lists T(s)...
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/_floor_isqrt_u64.h"

/* Dynamic cache of the triangle values T(s)(n,k). */
static seq_t **dp = NULL;            /* dp[n][k] */
static idx_t  dp_rows = 0;           /* number of rows currently allocated */

/* Cache of the sequence s(i) = Catalan(i−1) (s(0)=1). */
static seq_t *s_arr  = NULL;
static idx_t  s_cap  = 0;

/* Ensure that the triangle is computed up to row 'need' (inclusive). */
static void ensure_rows(idx_t need)
{
    if (dp_rows > need)
        return;

    /* Allocate a new array of row pointers. */
    size_t new_cap = (size_t)(need + 1);
    seq_t **new_dp = (seq_t**)malloc(new_cap * sizeof(seq_t*));
    if (!new_dp) exit(1);

    /* Copy existing row pointers. */
    size_t i;
    for (i = 0; i < dp_rows; ++i)
        new_dp[i] = dp[i];

    /* Allocate new rows if needed. */
    for (; i < new_cap; ++i)
        new_dp[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));

    /* Keep a copy of the old pointer array and free it. */
    free(dp);
    seq_t **old_dp = dp;
    dp = new_dp;
    idx_t old_rows = dp_rows;
    dp_rows = new_cap;

    /* Grow the s(i) cache if required. */
    if (s_cap < dp_rows) {
        seq_t *new_s = (seq_t*)realloc(s_arr, dp_rows * sizeof(seq_t));
        for (i = s_cap; i < dp_rows; ++i) {
            if (i == 0)
                new_s[i] = (seq_t)1;           /* value for s(0), not used elsewhere */
            else
                new_s[i] = catalan((idx_t)i - 1); /* s(i) = Catalan(i−1) */
        }
        s_arr = new_s;
        s_cap = dp_rows;
    }

    /* Initialize the new rows. */
    for (idx_t n = old_rows; n < dp_rows; ++n) {
        seq_t *row = dp[n];
        row[0] = (n == 0) ? (seq_t)1 : (seq_t)0;

        for (idx_t k = 1; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= n; ++i) {
                idx_t rem = n - i;
                if (rem < (k - 1))
                    continue; /* dp[rem][k-1] is defined only when rem >= k-1 */

                seq_t bin = binomial((idx_t)n - 1, i - 1);
                seq_t prod = bin * s_arr[i] * dp[rem][k - 1];
                sum += prod;
            }
            row[k] = sum;
        }
    }

    /* The initial row is already set in the loop above. */
}

/* The function required by the build. */
seq_t a(idx_t n)
{
    /* Determine which row and column of the triangle correspond to the
       flattened index n.  The n-th flattened index lies in row m with
       column k. */
    idx_t t = _floor_isqrt_u64((idx_t)8 * n + 1);
    idx_t m = (t - 1) / 2;

    idx_t offset = (idx_t)m * (m + 1) / 2;
    idx_t k = n - offset;

    /* Compute necessary rows of the triangle. */
    ensure_rows(m);

    return dp[m][k];
}
