/*
 * A304836  a(n) = 27*n^2 - 51*n + 24, n>=1.
 * https://oeis.org/A304836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*n^2 - 51*n + 24
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part = (seq_t)27 * nn * nn + (seq_t)24;
    return part - (seq_t)51 * nn;
}
