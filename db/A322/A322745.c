/*
 * A322745  a(n) = n * (16*n^2+20*n+5)^2.
 * https://oeis.org/A322745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (16*n^2+20*n+5)^2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 27, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t t = (seq_t)16 * m * m + (seq_t)20 * m + (seq_t)5;
    return m * t * t;
}
