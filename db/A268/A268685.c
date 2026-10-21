/*
 * A268685  a(n) = 3*(n + 1)*(n + 2)*(3*n + 1)*(3*n + 4)/4.
 * https://oeis.org/A268685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n + 1)*(n + 2)*(3*n + 1)*(3*n + 4)/4
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 156, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = 3 * (nn + 1) * (nn + 2);
    term *= (3 * nn + 1);
    term *= (3 * nn + 4);
    term /= 4;
    return term;
}
