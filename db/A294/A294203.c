/*
 * A294203  Number of partitions of n into distinct Lucas parts (A000204)
 *          greater than 1.
 * https://oeis.org/A294203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct Lucas parts > 1
 *            (Lucas numbers as in A000204: 1, 3, 4, 7, 11, ...; 1 is
 *            excluded)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 (empty partition) */
    if (n == 0)
        return 1;

    /* Allocate DP array for sums 0..n */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp)
        return 0;            /* Allocation failed: return 0 as safe value */

    dp[0] = 1;                /* empty partition */

    /* Generate Lucas numbers > 1 and update DP */
    idx_t l_prev = 2;        /* Lucas(0) */
    idx_t l_curr = 1;        /* Lucas(1) */

    while (1) {
        idx_t l_next = l_prev + l_curr;   /* next Lucas number */
        if (l_next > n)
            break;
        if (l_next > 1) {                /* parts > 1 only */
            for (idx_t j = n; j >= l_next; --j) {
                dp[j] += dp[j - l_next];
                if (j == l_next)          /* avoid unsigned wrap‑around */
                    break;
            }
        }
        l_prev = l_curr;
        l_curr = l_next;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
