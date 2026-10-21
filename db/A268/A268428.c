/*
 * A268428  a(n) = (3*(n^2+n+99)+cos(Pi*n/2)-sin(Pi*n/2))/2.
 * https://oeis.org/A268428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(3*n*(n+1)/4) + 149
 * Range:     u8 n <= 7, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * ((seq_t)n + 1) * 3;
    seq_t f = t / 4;            /* floor division */
    return f * 2 + (seq_t)149;
}
