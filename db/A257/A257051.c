/*
 * A257051  a(n) = cpg(n, 3) + cpg(n, 4) + ... + cpg(n, n) where cpg(n, m) is
 *          the m-th n-th-order centered polygonal number.
 * https://oeis.org/A257051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4-n^2-12)/6 for n>1, a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n4 = n2 * n2;
    seq_t numer = n4 - n2 - (seq_t)12;
    return numer / (seq_t)6;
}
