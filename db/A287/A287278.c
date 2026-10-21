/*
 * A287278  Number of set partitions of [n] such that for each block all
 *          absolute differences between consecutive elements are <= six.
 * https://oeis.org/A287278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of set partitions of [n] such that all absolute
 *            differences between consecutive elements of each block are <= 6.
 *            Computed by dynamic programming using a 6‑bit state that keeps
 *            track of the positions (relative to the current element) of the
 *            last element of each active block
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return (seq_t)1;

    /* dp[mask] – number of ways after processing the first i‑1
       elements, where mask (6 bits) records which positions
       relative to the next element are the last elements of
       active blocks.  Bit d (0‑based) is set if the last element
       of some block is i‑1‑d.  Initially no elements processed. */
    seq_t dp[64] = {0};
    dp[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t newdp[64] = {0};

        for (int mask = 0; mask < 64; ++mask)
        {
            seq_t cur = dp[mask];
            if (cur == (seq_t)0)
                continue;

            /* 1. start a new block using element i */
            int base = ((mask << 1) | 1) & 63;
            newdp[base] += cur;

            /* 2. add element i to one of the existing active blocks */
            for (int d = 0; d < 6; ++d)
            {
                if (mask & (1 << d))
                {
                    int mask_without = mask & ~(1 << d);
                    int nxt = ((mask_without << 1) | 1) & 63;
                    newdp[nxt] += cur;
                }
            }
        }

        /* copy newdp into dp for the next iteration */
        for (int mask = 0; mask < 64; ++mask)
            dp[mask] = newdp[mask];
    }

    /* Sum over all possible final states */
    seq_t total = (seq_t)0;
    for (int mask = 0; mask < 64; ++mask)
        total += dp[mask];
    return total;
}
