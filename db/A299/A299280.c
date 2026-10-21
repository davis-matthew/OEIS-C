/*
 * A299280  Partial sums of A299279.
 * https://oeis.org/A299280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; For n>0 even: a(n) = (5*n^3 + 8*n^2 + 6*n - 6)/2 For
 *            n>0 odd : a(n) = (5*n^3 + 7*n^2 + 5*n + 1)/2
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t un = (seq_t)n;
    seq_t n2 = un * un;
    seq_t n3 = n2 * un;

    if ((n & 1) == 0) {            /* even n */
        return (5 * n3 + 8 * n2 + 6 * un - 6) / 2;
    } else {                       /* odd n */
        return (5 * n3 + 7 * n2 + 5 * un + 1) / 2;
    }
}
