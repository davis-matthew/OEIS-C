/*
 * A305070  a(n) = 378*n^2 - 54*n (n>=1).
 * https://oeis.org/A305070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 378*n^2 - 54*n
 * Range:     u8 none, u16 n <= 13, u32 n <= 3370, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)378 * nn * nn;
    res -= (seq_t)54 * nn;
    return res;
}
