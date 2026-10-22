/*
 * A308927  Sum of the smallest parts in the partitions of n into 7 parts.
 * https://oeis.org/A308927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{s=1..⌊n/7⌋} s * p_{≤6}(n-7s) where p_{≤6}(m) is the
 *            number of partitions of m into parts of size ≤ 6
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Grow the partition table if needed */
    if (n >= dp_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (new_dp == NULL)
            return 0;  /* allocation failed */

        for (idx_t i = 0; i < new_size; ++i)
            new_dp[i] = 0;
        new_dp[0] = 1;

        /* Compute partitions into parts of size ≤ 6 */
        for (uint64_t s = 1; s <= 6; ++s)
        {
            for (idx_t i = s; i < new_size; ++i)
                new_dp[i] += new_dp[i - s];
        }

        dp = new_dp;
        dp_size = new_size;
    }

    /* Compute the sum over smallest parts */
    seq_t sum = 0;
    idx_t max_s = n / 7;
    for (idx_t s = 1; s <= max_s; ++s)
    {
        idx_t m = n - 7 * s;
        sum += (seq_t)s * dp[m];
    }
    return sum;
}
