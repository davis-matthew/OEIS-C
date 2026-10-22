/*
 * A387458  a(n) = 4*(23 - 17*n + 8*n^2).
 * https://oeis.org/A387458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(23 - 17*n + 8*n^2)
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* 4 * (23 - 17*n + 8*n^2) */
    return 4 * ( (seq_t)23 + (seq_t)8 * nn * nn - (seq_t)17 * nn );
}
