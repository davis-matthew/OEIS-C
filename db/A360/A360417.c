/*
 * A360417  a(n) = 8*n^2 - 7*n + 2.
 * https://oeis.org/A360417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 7*n + 2
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 8 * tn * tn - 7 * tn + 2;
}
