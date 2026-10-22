/*
 * A348644  a(n) = (18*n + 1)*(24*n + 1)*(144*n + 11).
 * https://oeis.org/A348644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n + 1)*(24*n + 1)*(144*n + 11)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = 18 * nn + 1;
    seq_t t2 = 24 * nn + 1;
    seq_t t3 = 144 * nn + 11;
    return t1 * t2 * t3;
}
