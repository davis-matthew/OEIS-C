/*
 * A242658  a(n) = 3*n^2 - 3*n + 2.
 * https://oeis.org/A242658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 - 3*n + 2
 * Range:     u8 n <= 9, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t res  = 3 * nn * nn;
    res       -= 3 * nn;
    res       += 2;
    return res;
}
