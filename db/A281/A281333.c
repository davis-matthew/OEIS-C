/*
 * A281333  a(n) = 1 + floor(n/2) + floor(n^2/3).
 * https://oeis.org/A281333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n/2) + floor(n^2/3)
 * Range:     u8 n <= 26, u16 n <= 442, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)1 + nn / (seq_t)2 + (nn * nn) / (seq_t)3;
}
