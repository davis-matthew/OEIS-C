/*
 * A327707  The minimal size of a partition lambda of n such that every
 *          partition of n with at most 7 parts can be obtained by coalescing
 *          the parts of lambda.
 * https://oeis.org/A327707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(floor(n * 6 / 7)), a(0) = 0
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t res = 0;
    while (m)
    {
        ++res;                 /* count one step */
        m = (m * 6) / 7;       /* next argument in the recurrence */
    }
    return res;
}
