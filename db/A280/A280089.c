/*
 * A280089  a(n) = 4*n^3 - 3*n + 1.
 * https://oeis.org/A280089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 3*n + 1
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 4 * nn * nn * nn;
    res -= 3 * nn;
    res += 1;
    return res;
}
