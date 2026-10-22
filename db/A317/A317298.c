/*
 * A317298  a(n) = (1/2)*(1 + (-1)^n + 2*n + 4*n^2).
 * https://oeis.org/A317298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(1 + (-1)^n + 2*n + 4*n^2)
 * Range:     u8 n <= 11, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 2*nn*nn + nn;          /* 2*n^2 + n  */
    if ((n & 1) == 0)                  /* add 1 when n is even */
        val += 1;
    return val;
}
