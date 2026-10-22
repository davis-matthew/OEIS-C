/*
 * A325124  Number of divisible pairs of positive integers up to n with at
 *          least one binary carry.
 * https://oeis.org/A325124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of pairs (i,j) with 1 ≤ i ≤ j ≤ n, i divides j and
 *            (i & j) != 0, i.e. at least one overlapping 1‑bit
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        for (idx_t j = i; j <= n; j += i)   /* j is a multiple of i */
        {
            if ((i & j) != 0)
                count++;                     /* (i,j) has a binary carry */
        }
    }
    return count;
}
