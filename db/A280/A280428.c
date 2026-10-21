/*
 * A280428  a(n) = 1729*n^3.
 * https://oeis.org/A280428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1729*n^3
 * Range:     u8 none, u16 n <= 3, u32 n <= 135, u64 n <= 303, u128 n <= 303,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * nn * 1729;
}
