/*
 * A366151  a(n) = T(n, 3), where T(n, k) = Sum_{i=0..n} i^k * binomial(n, i)
 *          * (1/2)^(n-k).
 * https://oeis.org/A366151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+3)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * (sn + (seq_t)3);
}
