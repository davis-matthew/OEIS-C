/*
 * A256315  Number of partitions of 3n into exactly 6 parts.
 * https://oeis.org/A256315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 3*n into exactly 6 parts (i.e.
 *            the number of partitions of m = 3*n into exactly k = 6 parts)
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;
static idx_t dp_cap = 0;   /* maximum m for which dp is filled (inclusive) */

/* Ensure dp contains rows 0..max_m and cols 0..6.
   dp_cap will be updated to max_m. */
static void ensure_dp(idx_t max_m)
{
    if (dp != NULL && dp_cap >= max_m)
        return;

    int had_old = (dp != NULL) ? 1 : 0;
    idx_t old_cap = had_old ? dp_cap : (idx_t)0;

    size_t new_len = (size_t)(max_m + 1) * 7;
    seq_t *new_dp = (seq_t *)calloc(new_len, sizeof(seq_t));
    if (!new_dp)
        return;   /* allocation failure; caller will get 0 */

    if (had_old) {
        size_t old_len = (size_t)(old_cap + 1) * 7;
        memcpy(new_dp, dp, old_len * sizeof(seq_t));
        free(dp);
    }

    /* Base row: dp[0][j] = 1 for all j */
    for (int j = 0; j <= 6; ++j)
        new_dp[j] = (seq_t)1;

    /* Compute new rows from old_cap + 1 up to max_m */
    idx_t start_row = had_old ? old_cap + 1 : 1;
    for (idx_t i = start_row; i <= max_m; ++i) {
        seq_t *row = new_dp + i * 7;
        row[0] = (seq_t)0;   /* dp[i][0] = 0 for i>0 */
        for (int j = 1; j <= 6; ++j) {
            seq_t val = row[j - 1];
            if (i >= (idx_t)j)
                val += new_dp[(i - (idx_t)j) * 7 + j];
            row[j] = val;
        }
    }

    dp = new_dp;
    dp_cap = max_m;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    idx_t m_needed = 3 * n - 6;    /* m = 3n - 6 */
    ensure_dp(m_needed);
    if (dp == NULL)
        return (seq_t)0; /* allocation failure */

    return dp[m_needed * 7 + 6];
}
