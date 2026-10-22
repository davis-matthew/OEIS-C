/*
 * A395241  a(n) = n^2*(4*n + 3).
 * https://oeis.org/A395241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(4*n + 3)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn * (((seq_t)4 * nn) + (seq_t)3);
    return res;
}
