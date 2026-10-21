/*
 * A254874  a(n) = floor((10*n^3 + 63*n^2 + 126*n + 89) / 72).
 * https://oeis.org/A254874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((10*n^3 + 63*n^2 + 126*n + 89) / 72)
 * Range:     u8 n <= 0, u16 n <= 16, u32 n <= 1026, u64 n <= 2500,
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
    seq_t num = 10 * n3 + 63 * n2 + 126 * nn + 89;
    return num / 72;
}
