/*
 * A383070  Number of n-digit positive integers where all pairs of consecutive
 *          digits have a difference of at least 2.
 * https://oeis.org/A383070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of n-digit positive integers where the absolute
 *            difference between every pair of consecutive digits is at least
 *            2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset starts at 1 */

    seq_t cur[10] = {0};
    seq_t nxt[10] = {0};

    /* Initial counts for 1‑digit numbers (digits 1–9). */
    for (int d = 1; d <= 9; ++d)
        cur[d] = 1;

    /* Build up to length n using dynamic programming. */
    for (idx_t pos = 2; pos <= n; ++pos)
    {
        for (int d = 0; d < 10; ++d)
        {
            seq_t sum = 0;
            for (int pd = 0; pd < 10; ++pd)
            {
                int diff = pd - d;
                if (diff < 0)
                    diff = -diff;
                if (diff >= 2)
                    sum += cur[pd];
            }
            nxt[d] = sum;
        }
        for (int d = 0; d < 10; ++d)
            cur[d] = nxt[d];
    }

    /* Total number of n‑digit numbers. */
    seq_t total = 0;
    for (int d = 0; d < 10; ++d)
        total += cur[d];
    return total;
}
