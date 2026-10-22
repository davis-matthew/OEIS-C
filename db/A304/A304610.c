/*
 * A304610  a(n) = 157*n - 40 (n>=1).
 * https://oeis.org/A304610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 157*n - 40 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 417, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using seq_t arithmetic to avoid overflow */
    return (seq_t)157 * (seq_t)n - (seq_t)40;
}
