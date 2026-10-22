/*
 * A391537  Guaranteed win if opponent chooses one of two baskets and the
 *          player chooses the coins with values from 1 to n (see Comments for
 *          details).
 * https://oeis.org/A391537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n-1)/4) + n
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1; 0 is not defined */
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    return nn * (nn - 1) / 4 + nn;
}
