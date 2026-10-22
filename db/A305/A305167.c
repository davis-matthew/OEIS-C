/*
 * A305167  Constant term in the expansion of (Sum_{k=0..n} k*(x^k +
 *          x^(-k)))^4.
 * https://oeis.org/A305167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)*(17*n^4+34*n^3+28*n^2+11*n+15)/105
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 sn = (unsigned __int128)n;

    unsigned __int128 a0 = sn;
    unsigned __int128 b0 = sn + 1;
    unsigned __int128 c0 = sn * 2 + 1;

    unsigned __int128 n2 = sn * sn;
    unsigned __int128 n3 = n2 * sn;
    unsigned __int128 n4 = n3 * sn;

    unsigned __int128 poly = (unsigned __int128)17 * n4
                          + (unsigned __int128)34 * n3
                          + (unsigned __int128)28 * n2
                          + (unsigned __int128)11 * sn
                          + 15;

    unsigned __int128 num = a0 * b0 * c0 * poly;
    unsigned __int128 res = num / 105;

    return (seq_t)res;
}
