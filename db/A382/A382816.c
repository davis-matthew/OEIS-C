/*
 * A382816  a(n) = number of occurrences of n in A008949.
 * https://oeis.org/A382816
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of occurrences of n in A008949 (partial sums of
 *            Pascal's triangle)
 * Range:     u8 n <= 44, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* offset is 2 */
    seq_t m      = (seq_t)n;             /* target integer */
    seq_t count  = 0;

    for (idx_t row = 1; row <= n - 1; ++row)  /* only rows 1 .. n-1 can contain n */
    {
        seq_t binom = 1;                   /* C(row,0) */
        seq_t sum   = 1;                   /* partial sum after k = 0 */

        if (sum == m) ++count;            /* check k = 0 */

        for (idx_t k = 1; k <= row; ++k)
        {
            binom = binom * (seq_t)(row - k + 1) / (seq_t)k;   /* C(row,k) */
            sum  += binom;
            if (sum == m) ++count;
            if (sum > m) break;            /* sums increase monotonically in k */
        }
    }

    return count;
}
