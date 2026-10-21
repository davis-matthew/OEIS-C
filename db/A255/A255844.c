/*
 * A255844  a(n) = 2*n^2 + 6.
 * https://oeis.org/A255844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 6
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2 * n^2 + 6 using seq_t arithmetic. */
    return ( (seq_t)n * (seq_t)n * (seq_t)2 ) + (seq_t)6;
}
