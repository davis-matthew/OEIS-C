/*
 * A331768  El Gradechi's hybrid coefficients alpha^{4,4}_{2n}.
 * https://oeis.org/A331768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80*(3+2*n)*(2+2*n)*(1+2*n)
 * Range:     u8 none, u16 n <= 3, u32 n <= 187, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;                     /* convert index to seq_t */
    seq_t m  = (seq_t)3 + (seq_t)2 * ns;     /* 3 + 2n */
    seq_t m1 = m - (seq_t)1;                 /* 2 + 2n */
    seq_t m2 = m - (seq_t)2;                 /* 1 + 2n */
    seq_t prod = m * m1;                     /* m*(m-1) */
    prod = prod * m2;                        /* m*(m-1)*(m-2) */
    return (seq_t)80 * prod;                 /* 80 * product */
}
