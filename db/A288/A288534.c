/*
 * A288534  a(n) = n*(2*n^2 + 3), n >= 1; a(0)=1.
 * https://oeis.org/A288534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (2*n*n + 3) for n>=1; a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t t  = (seq_t)2 * n2 + (seq_t)3;
    return nn * t;
}
