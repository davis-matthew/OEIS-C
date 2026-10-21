/*
 * A294141  Number of partitions of n into odd parts that do not divide n.
 * https://oeis.org/A294141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd parts that do not
 *            divide n
 * Range:     u8 n <= 46, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate working array dp[0..n], initialized to zero */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                /* allocation failure – unlikely for legal n */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Iterate over odd part sizes; skip those that divide n */
    for (idx_t part = 1; part <= n; part += 2)
    {
        if (n % part == 0)              /* forbidden part – it's an odd divisor of n */
            continue;

        for (idx_t sum = part; sum <= n; ++sum)
        {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
