/*
 * A374384  a(n) = floor(Sum_{k=n^3..(n+1)^3} k^(1/3)).
 * https://oeis.org/A374384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n^3 + (9*n^2)/2 + 4*n + 1)
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t term1 = (seq_t)3 * n3;
    seq_t term2 = (seq_t)9 * n2 / (seq_t)2;
    seq_t term3 = (seq_t)4 * sn;
    return term1 + term2 + term3 + (seq_t)1;
}
