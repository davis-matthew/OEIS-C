/*
 * A354008  Numerators of Cesàro means sequence of A114112.
 * https://oeis.org/A354008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 1;
 *          for n even:      a(n) = n + 1;
 *          for n odd > 1:   a(n) = ((n + 1)/2) * n + 1
 *          (i.e. a(2m+1) = (m+1)*(2m+1) + 1)
 */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* even n */
    if ((n & 1) == 0)
        return (seq_t)n + 1;

    /* odd n > 1 */
    idx_t m = (n - 1) / 2;          /* m = (n-1)/2 */
    seq_t m1 = (seq_t)(m + 1);      /* m + 1 */
    seq_t two_m1 = (seq_t)(2 * m + 1);/* 2m + 1  == n */
    return m1 * two_m1 + 1;         /* (m+1)*(2m+1)+1 */
}
