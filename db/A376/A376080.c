/*
 * A376080  a(n) is the highest degree of the rational function in the
 *          recursive substitution {y1, y2} -> {y2, (y2 + 1)/(y1*y2)} after n
 *          steps.
 * https://oeis.org/A376080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((3*n^2 - 3*n + 8)/14) initial values: a(0)=1,
 *            a(1)=1, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = 3 * nn * nn - 3 * nn + 8;
    return (num + 13) / 14;
}
