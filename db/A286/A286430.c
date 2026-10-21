/*
 * A286430  Least volume of water to surround the largest possible island in a
 *          number square.
 * https://oeis.org/A286430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n ≤ 3; a(n) = 8*n^2 - 30*n + 28 for n > 3
 * Range:     u8 n <= 7, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    seq_t ns = (seq_t)n;
    /* compute 8*n^2 - 30*n + 28 using only integer arithmetic */
    seq_t term = (seq_t)8 * ns * ns - (seq_t)30 * ns + (seq_t)28;
    return term;
}
