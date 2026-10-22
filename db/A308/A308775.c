/*
 * A308775  Sum of all the parts in the partitions of n into 4 parts.
 * https://oeis.org/A308775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p₄(n) where p₄(n) is the number of integer partitions
 *            of n into exactly four positive parts
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* For n < 4 no 4‑part partition exists. */
    if (n < 4)
        return 0;

    /* Compute m = n – 4 which is the sum for which we need the
       number of partitions into parts ≤ 4 (conjugate to exactly 4 parts). */
    idx_t m = n - 4;

    /* Static table holding p₄ values via partitions into parts ≤ 4. */
    static seq_t *dp = NULL;     /*  dp[i] = number of partitions of i with parts ≤ 4 */
    static idx_t max_m = 0;      /*  largest i for which dp is currently computed */

    /* Initialise the table on first entry. */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t));
        if (dp == NULL)
            return 0;           /* unlikely, but defensive */
        dp[0] = 1;              /* empty partition */
        max_m = 0;
    }

    /* Recompute dp up to new maximum m when necessary. */
    if (m > max_m) {
        size_t size = (size_t)(m + 1);
        dp = (seq_t *)realloc(dp, size * sizeof(seq_t));
        if (dp == NULL)
            return 0;           /* unlikely */
        dp[0] = 1;
        for (idx_t i = 1; i <= m; ++i)
            dp[i] = 0;

        /* Standard coin‑change DP: parts 1,2,3,4. */
        for (int part = 1; part <= 4; ++part) {
            for (idx_t i = part; i <= m; ++i) {
                dp[i] += dp[i - part];
            }
        }
        max_m = m;
    }

    /* a(n) = n * dp[n-4] */
    return (seq_t)n * dp[m];
}
