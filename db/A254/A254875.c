/*
 * A254875  a(n) = floor((10*n^3 + 57*n^2 + 102*n + 72) / 72).
 * https://oeis.org/A254875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((10*n^3 + 57*n^2 + 102*n + 72) / 72)
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 967, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = (seq_t)10 * n3 + (seq_t)57 * n2 + (seq_t)102 * nn + (seq_t)72;
    return num / (seq_t)72;
}
